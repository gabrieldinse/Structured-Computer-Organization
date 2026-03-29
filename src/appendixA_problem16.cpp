#include "appendixA_problem16.hpp"

namespace SCO {
// Both numA and numB have the same length
Result binarySum(const std::string &numA, const std::string &numB) {
  int carry = 0;
  int prevCarry = 0;
  Result result{std::string(numA.size(), 0)};

  for (int i = static_cast<int>(numA.size()) - 1; i >= 0; i--) {
    int sum = (numA[i] - '0') + (numB[i] - '0') + carry;
    result.sum[i] = (sum % 2) + '0';
    prevCarry = carry;
    carry = sum / 2;
  }

  result.overflow = carry != prevCarry;
  return result;
}
} // namespace SCO
