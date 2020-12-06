#ifndef _H_MOTOR_ENCODER_
#define _H_MOTOR_ENCODER_

#include <array>

#include "pin.hpp"
#include "events.hpp"
#include "motor-utils.hpp"


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

  // We have lots of cases where we need arrays with one entry for
  // each motor instance (LEFT and RIGHT).
  template<typename T>
  using InstanceArray = std::array<T, 2>;

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


  // Current valid capture count.
  uint8_t valid_samples(Instance instance) const { return _valid_samples[instance]; }

  // Current overcapture flags.
  bool overcapture(Instance instance) const { return _overcapture[instance]; }

  // Clear overcapture flags.
  void clear_overcapture(Instance instance) { _overcapture[instance] = false; }

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
  InstanceArray<int> _chs;

  // Wheel/encoder information.
  WheelInfo _wheel_info;
  float _tooth_distance_um;

  // GPIO pins used as pulse inputs: left, then right.
  InstanceArray<Pin> _pins;

  // Main sample buffers (circular).
  InstanceArray<SampleBuffer> _edge_times;

  // Overcapture error flags.
  InstanceArray<bool> _overcapture = { false, false };

  // Next sample position in circular sample buffers.
  InstanceArray<uint8_t> _sample_idx = {0, 0};

  // Count of valid edge captures in each sample buffer.
  InstanceArray<uint8_t> _valid_samples = {0, 0};

  // Polarity of last observed edge for each input pin.
  InstanceArray<Edge> _last_edge = {FALLING, FALLING};

  bool _inited = false;
  bool _event_loop_started = false;

  void discard_edges(Instance i, uint32_t now);
};

};

#endif
