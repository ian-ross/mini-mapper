#ifndef _H_MOTOR_TORQUE_
#define _H_MOTOR_TORQUE_

#include <array>

#include "dma.hpp"
#include "pin.hpp"
#include "pwm.hpp"
#include "events.hpp"
#include "motor-utils.hpp"


namespace Motor {

// Measure motor torque by current sensing for multiple motors:
//
//  * Multiple motor instances can be added by giving the pin used for
//    the motor current sense input. Instances are indexed by an
//    Instance enumeration type.
//
//  * Torque measurements are performed at a fixed frequency, with
//    multiple measurements being retained for smoothing.
//
//  * Sense currents (converted to voltages and amplified) are
//    collected by ADC conversions triggered by a timer. The ADC is
//    configured to collect one sample for each defined motor
//    instance. DMA is used to transfer ADC conversion results to a
//    buffer.

class Torque {
public:

  // We collect a number of samples into a circular buffer for
  // smoothing.
  static const int SAMPLE_COUNT = 32;

  // We keep SAMPLE_COUNT samples at a time for smoothing. Each sample
  // is a 12-bit ADC result stored in a 16-bit half-word.
  using SampleBuffer = std::array<uint16_t, SAMPLE_COUNT>;

  // Calibration for converting ADC counts to current and torque
  // values.
  class Calibration {
    public:
    Calibration();

    // Return current (mA) for a given ADC count.
    float current(float adc_count) const;

    // Return torque (N mm) for a given ADC count.
    float torque(float adc_count) const;
  };

  // TODO: CALIBRATION FOR CONVERSION FROM ADC COUNTS TO TORQUE
  // VALUES.

  Torque(TIM_TypeDef *timer, ADC_TypeDef *adc, DMAChannel &dma,
         int measure_interval_ms, const Calibration &calib,
         const Pin &left_pin, const Pin &right_pin);

  // Initialise timer, ADC and DMA.
  void init(void);

  // To be called from DMA transfer complete IRQ.
  void dma_transfer_complete_irq(void);

  // Return latest sample for instance.
  uint16_t latest(Instance instance);

  // Return (box-car) smoothed samples for instance.
  float smoothed(Instance instance);

  // Return current and torque values based on calibration data.
  float latest_current(Instance i) { return _calib.current(latest(i)); }
  float smoothed_current(Instance i) { return _calib.current(smoothed(i)); }
  float latest_torque(Instance i) { return _calib.torque(latest(i)); }
  float smoothed_torque(Instance i) { return _calib.torque(smoothed(i)); }

  // Start and stop sample collection. Calling `start` clears sample
  // buffers.
  void start(void);
  void stop(void);

#ifndef TEST
  // Kind of shonky, but it makes life oh so much easier when
  // testing...
private:
#endif

  // MCU timer used to trigger ADC conversions.
  TIM_TypeDef *_timer;

  // Interval between ADC conversion rounds in milliseconds.
  int _interval_ms;

  // ADC instance used to perform current measurements.
  ADC_TypeDef *_adc;

  // DMA channel information for channel used to extract conversion
  // results from ADC to `_dma_buffer` buffer.
  const DMAChannel _dmac;

  // DMA controller peripheral.
  DMA_TypeDef *_dma;

  // Calibration data to convert ADC counts to current and torque
  // values.
  const Calibration &_calib;

  // DMA destination buffer.
  std::array<uint16_t, 2> _dma_buffer;

  // Main sample buffers (circular).
  std::array<SampleBuffer, 2> _samples;

  // Index pointing to latest sample in circular sample buffers:
  // sentinel -1 value means that the buffer has not been initialised,
  // so the first values added should be used to initialise all
  // entries in the buffer so that smoothing works correctly as the
  // buffer is filled.
  int _sample_idx = -1;

  // GPIO pins used as analog inputs: left, then right.
  std::array<Pin, 2> _pins;

  void configure_dma(void);
  void configure_timer(void);
  void configure_adc(void);
};

};

#endif
