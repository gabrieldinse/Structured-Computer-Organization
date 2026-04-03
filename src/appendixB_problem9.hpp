#pragma once

#include <cstdint>

namespace SCO {
struct IEEE754Float {
  std::uint32_t sign;
  std::uint32_t exponent;
  std::uint32_t significand;
};

IEEE754Float extract(float num);
float sum(float numA, float numB);
} // namespace SCO
