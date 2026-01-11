#pragma once

#include <print>
#include <vector>

namespace SCO {
template <typename T> std::size_t hammingWeight(T val) {
  std::size_t bitsSet = 0;
  while (val) {
    bitsSet += val & 1;
    val >>= 1;
  }
  return bitsSet;
}

template <typename T>
std::size_t distance(const std::vector<T> &code, std::size_t codewordLength) {
  if (code.size() < 2) {
    return codewordLength;
  }

  std::size_t minDistance = codewordLength;
  for (std::size_t i = 0; i < code.size(); i++) {
    for (std::size_t j = i + 1; j < code.size(); j++) {
      T diff = code[i] ^ code[j];
      std::size_t bitsSet = hammingWeight(diff);
      if (bitsSet < minDistance) {
        minDistance = bitsSet;
      }

      if (minDistance == 1) {
        return 1;
      }
    }
  }

  return minDistance;
}
} // namespace SCO
