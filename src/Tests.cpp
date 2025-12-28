#include <algorithm>
#include <print>

#include "gtest/gtest.h"

#include "Algorithms.hpp"
#include "Utils.hpp"

TEST(SCOTests, Hamming) {
  struct TestCase {
    char input;
    std::int16_t expected;
  };

  std::vector<TestCase> testCases = {
      {'A', 1156}, // ASCII 65
      {'B', 1178}, // ASCII 66
      {'C', 1181}, // ASCII 67
      {'D', 1193}, // ASCII 68
      {'E', 1198}, // ASCII 69
      {'F', 1200}, // ASCII 70
      {'G', 1207}, // ASCII 71
      {'H', 1224}  // ASCII 72
  };

  for (const auto &testCase : testCases) {
    std::int16_t result = SCO::hamming(testCase.input);
    EXPECT_EQ(result, testCase.expected)
        << "Failed for input: " << testCase.input;
  }
}

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