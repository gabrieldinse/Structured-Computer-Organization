#include "Algorithms.hpp"

namespace SCO {
// Chapter 2 - Problem 35
std::int16_t hamming(char ascii) {
  // p1 p2 d1 p3 d2 d3 d4 p4 d5 d6 d7
  char p1 = getBit(ascii, 0) ^ getBit(ascii, 1) ^ getBit(ascii, 3) ^
            getBit(ascii, 4) ^ getBit(ascii, 6);
  char p2 = getBit(ascii, 0) ^ getBit(ascii, 2) ^ getBit(ascii, 3) ^
            getBit(ascii, 5) ^ getBit(ascii, 6);
  char p3 = getBit(ascii, 1) ^ getBit(ascii, 2) ^ getBit(ascii, 3);
  char p4 = getBit(ascii, 4) ^ getBit(ascii, 5) ^ getBit(ascii, 6);

  std::int16_t result = (p1 << 0) | (p2 << 1) | (getBit(ascii, 0) << 2) |
                        (p3 << 3) | (getBit(ascii, 1) << 4) |
                        (getBit(ascii, 2) << 5) | (getBit(ascii, 3) << 6) |
                        (p4 << 7) | (getBit(ascii, 4) << 8) |
                        (getBit(ascii, 5) << 9) | (getBit(ascii, 6) << 10);
  return result;
}

// Chapter 2 - Problem 36

} // namespace SCO
