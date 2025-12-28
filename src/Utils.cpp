#include "Utils.hpp"

namespace Utils {
std::vector<int> randomVector(size_t size, int min, int max) {
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(min, max);

  std::vector<int> elements(size);
  for (auto &element : elements) {
    element = dist(rng);
  }
  return elements;
}
} // namespace Utils