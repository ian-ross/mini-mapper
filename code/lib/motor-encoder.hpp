#ifndef _H_MOTOR_ENCODER_
#define _H_MOTOR_ENCODER_

#include <array>

#include "pin.hpp"
#include "events.hpp"
#include "motor-utils.hpp"


// QUESTIONS
//
// 1. How to do edge capture? Can you do capture on multiple pins
//    using the same timer? Can you do both rising and falling edges
//    at the same time?
//
//  - Timers with input capture are basically all timers except TIM6
//    and TIM7 basic timers.
//  - Timers with multiple input capture channels are TIM9, TIM12 (2
//    channels); TIM2, TIM3, TIM4, TIM5 (4 channels); TIM1, TIM8 (4
//    channels).
//  - Typical times we'll want to measure:
//      * Wheel turning at 90 rpm (max, equivalent to 28.3 cm/s linear
//        speed) => encoder turning at 90 x 48 = 4320 rpm = 72 Hz.
//      * Disk has 12 teeth => tooth frequency = 72 x 12 = 864 Hz.
//      * If teeth/gaps are same size, edge freq. = 864 x 2 = 1728 Hz.
//        (Teeth and gaps aren't same size, but this is indicative.)
//      * So time between edges will be 578.7 μs.
//  - If we have a timer with a prescaler set to give a count
//    frequency of 1 MHz, then a 16-bit counter will be able to
//    measure times of up to 65.536 ms. A 32-bit counter will be able
//    to measure times of up to 4294.97 s (i.e. 1 hr 11 min 34.97 s).
//  - At 1 mm/s linear speed, we see 1 / (60π) * 48 * 12 * 2 = 6.11
//    edges per second, i.e. an inter-edge time of 163.6 ms. Seems
//    like a 32-bit timer might be easier in this case...
//  - If we use a 32-bit timer, we can also reduce the prescaler to
//    get a higher count frequency and corresponding better timing
//    accuracy.
//  - The 32-bit timers are TIM2 and TIM5, both of which have four
//    capture channels.
//  - GPIO inputs for capture channels have to be assigned as specific
//    alternate functions. There seem to be enough redundant
//    assignments to make this easy enough to do.
//  - Can either get an interrupt on the capture event, or get the
//    capture timer count transferred via DMA and get an interrupt on
//    DMA transfer complete. (For our purposes, an interrupt on each
//    capture event is probably simplest to deal with, because we need
//    to look at the GPIO level to know whether we have a rising or
//    falling edge. In the ISR for the capture, we can check the GPIO
//    level, bundle that up with the capture count and the channel
//    we're looking at, and emit an event carrying all that
//    information for later processing.)
//  - Capture edge polarity is controlled by the CCxP and CCxNP fields
//    in the TIMx->CCER register: setting both of these bits captures
//    both rising and falling edges.
//
// 2. What information do we want to make available? Ultimately we
//    want to provide a speed measurement, possibly with different
//    kinds of smoothing (none: latest instantaneous value, boxcar
//    smoothing over sample time, exponentially weighted smoothing?),
//    but we also probably want access to raw time values as well,
//    i.e. times of the last N edge transitions.
//
//  - For a single encoder, we expect to get a sequence of alternating
//    rising and falling edges. We'll store the edge type (rising or
//    falling) and the timer count when the edge occurred. We'll keep
//    the last N edge times, retiring eges when they get too old or we
//    need to reuse storage space.
//  - We want to be able to retrieve time differences between edges
//    and to convert them into rotation rates and linear speeds, both
//    on an instantaneous basis and smoothed over the collected data
//    (for the last 500 ms or something similar?).
//  - To start with, let's provide functions to retrieve time
//    differences between adjacent edges (i.e. of opposite polarity)
//    and between adjacent edges of the same polarity.
//
// 3. What sort of time resolution do we need? And what to do about
//    the really slow cases when the wheel is more or less stopped? Do
//    we need some sort of rollover counter to time longer periods?
//
//  - A single edge corresponds to 60π / 48 / 12 / 2 = 0.16 mm
//    (assuming symmetry of rising and falling edges).
//  - Using a 32-bit timer running at 1 MHz, we can get 1 μs
//    resolution for each edge. At maximum speed (90 rpm for the
//    wheel, i.e. 60π * 1.5 = 282.74 mm/s), 1 μs corresponds to a
//    linear distance of 0.28 μm. At lower speeds, the linear distance
//    resolution is correspondingly finer.
//  - As noted above, with a timer count frequency of 1 MHz, a 32-bit
//    counter is able to measure times of up to 4294.97 s (i.e. 1 hr
//    11 min 34.97 s).
//  - Both of these factors indicate that a 1 MHz 32-bit timer is
//    pretty huge overkill... So let's go with that then!

namespace Motor {

  class Encoder : public Events::Consumer {
public:

  // We collect a number of samples into a circular buffer for
  // smoothing.
  static const int SAMPLE_COUNT = 32;

  // Time horizon for discarding captured edges (μs).
  static const uint32_t EDGE_DISCARD_TIME = 50000;

  // We keep SAMPLE_COUNT samples at a time for smoothing. Each sample
  // is a 32-bit timer capture result.
  using SampleBuffer = std::array<uint32_t, SAMPLE_COUNT>;

  // All edges from the photoencoder input are recording, and we keep
  // track of which polarity each timer capture corresponds to.
  enum Edge { RISING, FALLING };

  // Averaging modes for speed calculations:
  //
  //  - LAST_EDGES uses the last two captures, whatever edges they
  //    come from, and assumes they come from edges that are
  //    distributed evenly about the encoder disk (i.e. 24 edges at 15
  //    degree intervals).
  //  - LAST_TOOTH uses the last two captures that are from the same
  //    edge polarity and assumes that they come from the
  //    corresponding edges of adjacent teeth on the encoder disk
  //    (i.e. 12 teeth at 30 degree intervals).
  //  - BOXCAR uses all the available interval data equally weighted
  //    to produce a smoothed speed measurement over the full "live"
  //    measurement time interval.
  //  - EXPONENTIAL uses all the available interval data,
  //    exponentially weighted to give greater weight to more recent
  //    measurements, to produce a smoothed speed measurement over the
  //    full "live" measurement time interval.
  enum Averaging { LAST_EDGES, LAST_TOOTH, BOXCAR, EXPONENTIAL };

  // Information about the wheel and encoder.
  struct WheelInfo {
    // Wheel circumference (μm).
    uint32_t circumference;

    // Gear ratio between shaft with encoder disk and wheel (encoder
    // disk revolutions per wheel revolution).
    uint8_t encoder_gear_ratio;

    // Number of teeth on encoder disk.
    uint8_t encoder_teeth;
  };

  // We initialise an encoder by giving the timer to use for input
  // captures, the two motor pulse pins (one for the left and one for
  // the right), and information about the wheel and encoder disk
  // (used for converting pulse counts to linear velocities).
  Encoder(TIM_TypeDef *timer, const Pin &left_pin, const Pin &right_pin,
          const WheelInfo &wheel_info);

  // Check and initialise hardware resources for encoder: GPIO inputs,
  // timer timebase, timer capture channels.
  void init(void);

  // Events::Consumer dispatcher.
  virtual void dispatch(const Events::Event &e) override;

  // ISR for timer capture events.
  void timer_capture_irq(void);


  // Return inter-capture interval (μs) for wheel instance with
  // specified averaging mode.
  float interval(Averaging avg_mode, Instance instance) const;

  // Return last edge (rising or falling) for wheel instance.
  Edge last_edge(Instance instance) const;

  // Return linear speed (mm/s) for wheel instance with specified
  // averaging mode.
  float speed(Averaging avg_mode, Instance instance) const;

#ifndef TEST
  // Kind of shonky, but it makes life oh so much easier when
  // testing...
private:
#endif

  // Timer used for input captures.
  TIM_TypeDef *_timer;

  // Timer capture channels.
  std::array<int, 2> _chs;

  // Wheel/encoder information.
  WheelInfo _wheel_info;

  // GPIO pins used as pulse inputs: left, then right.
  std::array<Pin, 2> _pins;

  // Main sample buffers (circular).
  std::array<SampleBuffer, 2> _edge_times;

  // Next sample position in circular sample buffers.
  std::array<uint8_t, 2> _sample_idx = {0, 0};

  // Count of valid edge captures in each sample buffer.
  std::array<uint8_t, 2> _valid_samples = {0, 0};

  // Polarity of last observed edge for each input pin.
  std::array<Edge, 2> _last_edge = {FALLING, FALLING};

  void discard_edges(Instance i, uint32_t now);
};

};

#endif
