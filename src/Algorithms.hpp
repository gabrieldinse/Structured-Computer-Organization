#pragma once

#include <cstdint>

namespace SCO {
template <typename T> T getBit(T num, std::size_t pos) {
  return (num >> pos) & 1;
}

// Chapter 2 - Problem 35
std::int16_t hamming(char ascii);
} // namespace SCO
