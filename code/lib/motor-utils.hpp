#ifndef _H_MOTOR_UTILS_
#define _H_MOTOR_UTILS_

#include <stdint.h>
#include <array>


namespace Motor {

// We support two motor instances, one for the left motor and one for
// the right.
enum Instance : uint8_t {
  LEFT = 0,
  RIGHT
};

const std::array<Instance, 2> INSTANCES = { LEFT, RIGHT };

};

#endif
