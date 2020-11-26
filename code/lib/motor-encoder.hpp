#ifndef _H_MOTOR_
#define _H_MOTOR_

#include <array>

#include "pin.hpp"
#include "events.hpp"


namespace Motor {

#if 0
class Encoder {
public:

  static const int SAMPLE_COUNT = 32;

  enum Edge { RISING, FALLING };

  // We need to give a pin here, and we want to capture times between
  // edges, so we need a timer too.
  //
  // (Might also need some other timer configuration parameters.)
  //
  // Also need something to determine how many edge times to keep for
  // averaging?

  // int(pi * 60 * 1000) = 188495
  //
  // Motor::Encoder left_encoder(PB5, TIM9, 188495);

  Encoder(Pin &pulse, TIM_TypeDef *timer, int wheel_circumference_microns);

  // QUESTIONS:
  //
  //  - How will you want to use this? Returning a single interval
  //    gives you some sort of instantaneous measure of speed, but
  //    that might not be useful a lot of the time.
  //  - Define averaging for speed over a fixed time interval rather
  //    than a fixed set of intervals?
  //  - Might need to write some code using this to make the API
  //    concrete.
  //  - Start with an API oriented toward getting the hardware
  //    working: measure individual intervals between edges from the
  //    photoencoder, and once that's working, think about what might
  //    be needed in terms of speed calculations.

  int last_full_tooth_interval(void) const;
  int last_interval(void) const;
  Edge last_interval_edge(void) const;
  int speed_mm_per_s(void) const;

private:

  std::array<int, SAMPLE_COUNT> _edge_intervals;
  Edge _last_edge = FALLING;
};
#endif

};

#endif
