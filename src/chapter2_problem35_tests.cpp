#include <print>

#include "gtest/gtest.h"

#include "chapter2_problem35.hpp"

namespace SCO {
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
} // namespace SCO