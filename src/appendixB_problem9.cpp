#include <bit>

#include "appendixB_problem9.hpp"

namespace SCO {
IEEE754Float unpack(float packed) {
  auto bits = std::bit_cast<uint32_t>(packed);
  return IEEE754Float{.sign = (bits >> 31) & 0x1,
                      .exponent = (bits >> 23) & 0xFF,
                      .significand = (bits) & 0x7FFFFF};
}

float pack(IEEE754Float unpacked) {
  return std::bit_cast<float>((unpacked.sign << 31) |
                              ((unpacked.exponent & 0xFF) << 23) |
                              (unpacked.significand & 0x7FFFFF));
}

float sum(float numA, float numB) {
  // 1. Extract sign, exponent, significand from both numbers
  IEEE754Float rawNumA = unpack(numA);
  IEEE754Float rawNumB = unpack(numB);
  IEEE754Float result;

  if (std::bit_cast<std::uint32_t>(numA) == 0x0) {
    return numB;
  }

  if (std::bit_cast<std::uint32_t>(numB) == 0x0) {
    return numA;
  }

  // 2. Align exponents → shift smaller number right until exponents match
  rawNumB.significand = rawNumB.significand | (1 << 23);
  rawNumA.significand = rawNumA.significand | (1 << 23);
  if (rawNumA.exponent > rawNumB.exponent) {
    while (rawNumA.exponent != rawNumB.exponent) {
      rawNumB.significand >>= 1;
      rawNumB.exponent++;
    }
  } else {
    while (rawNumA.exponent != rawNumB.exponent) {
      rawNumA.significand >>= 1;
      rawNumA.exponent++;
    }
  }

  result.exponent = rawNumA.exponent; // Both exponents are now the same

  // 3. Add (or subtract) significands
  if (rawNumA.sign == rawNumB.sign) {
    result.significand = rawNumA.significand + rawNumB.significand;
    result.sign = rawNumA.sign; // Both signs are the same
  } else {
    if (rawNumA.significand >= rawNumB.significand) {
      result.significand = rawNumA.significand - rawNumB.significand;
      result.sign = rawNumA.sign;
    } else {
      result.significand = rawNumB.significand - rawNumA.significand;
      result.sign = rawNumB.sign;
    }
  }

  // 4. Renormalize result → shift until form is 1.something
  if (result.significand) {
    if (result.significand & (1 << 24)) { // If we have a carry bit, shift right
      result.significand >>= 1;
      result.exponent++;
    } else {
      while (not(result.significand & (1 << 23))) {
        result.significand <<= 1;
        result.exponent--;
      }
    }

    // Remove the implicit leading 1 bit
    result.significand = result.significand & 0x7FFFFF;
  } else {
    result.exponent = 0;
    result.sign = 0; // Sign of zero is positive
  }

  // 5. Pack back into IEEE format
  float packed = pack(result);

  // 7. Check for overflow/underflow
  return packed;
}
} // namespace SCO
