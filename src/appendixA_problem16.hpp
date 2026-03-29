#pragma once

#include <string>
#include <vector>

namespace SCO {
struct Result {
  std::string sum;
  bool overflow = false;
};

Result binarySum(const std::string &numA, const std::string &numB);
} // namespace SCO
