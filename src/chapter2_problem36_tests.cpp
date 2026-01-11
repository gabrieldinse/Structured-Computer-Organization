#include <print>

#include "gtest/gtest.h"

#include "chapter2_problem36.hpp"

namespace SCO {
TEST(SCOTests, Distance) {
  std::vector<int> code = {0b0000, 0b1111, 0b1010, 0b0101};
  int codewordLength = 4;

  int result = SCO::distance(code, codewordLength);
  EXPECT_EQ(result, 2);

  code = {0b000000, 0b111111, 0b000111, 0b111000};
  codewordLength = 6;
  result = SCO::distance(code, codewordLength);
  EXPECT_EQ(result, 3);
}
} // namespace SCO