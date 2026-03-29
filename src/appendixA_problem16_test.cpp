#include "appendixA_problem16.hpp"
#include "gtest/gtest.h"

namespace SCO {

// Basic addition, no overflow
TEST(BinarySumTest, ZeroPlusZero) {
  auto result = binarySum("0000", "0000");
  EXPECT_EQ(result.sum, "0000");
  EXPECT_FALSE(result.overflow);
}

TEST(BinarySumTest, OnePlusZero) {
  auto result = binarySum("0001", "0000");
  EXPECT_EQ(result.sum, "0001");
  EXPECT_FALSE(result.overflow);
}

TEST(BinarySumTest, SimpleSumNoCarry) {
  // 3 + 4 = 7
  auto result = binarySum("0011", "0100");
  EXPECT_EQ(result.sum, "0111");
  EXPECT_FALSE(result.overflow);
}

TEST(BinarySumTest, SumWithCarry) {
  // 1 + 1 = 2
  auto result = binarySum("0001", "0001");
  EXPECT_EQ(result.sum, "0010");
  EXPECT_FALSE(result.overflow);
}

// Two's complement positive overflow
// Positive + Positive = Negative (overflow)
TEST(BinarySumTest, PositiveOverflow) {
  // 4-bit: 7 + 1 = 8 (overflow, wraps to -8)
  // 0111 + 0001 = 1000
  auto result = binarySum("0111", "0001");
  EXPECT_EQ(result.sum, "1000");
  EXPECT_TRUE(result.overflow);
}

TEST(BinarySumTest, PositiveOverflow2) {
  // 4-bit: 5 + 4 = 9 (overflow)
  // 0101 + 0100 = 1001
  auto result = binarySum("0101", "0100");
  EXPECT_EQ(result.sum, "1001");
  EXPECT_TRUE(result.overflow);
}

// Two's complement negative overflow
// Negative + Negative = Positive (overflow)
TEST(BinarySumTest, NegativeOverflow) {
  // 4-bit: -8 + -1 = -9 (overflow, wraps to 7)
  // 1000 + 1111 = 0111
  auto result = binarySum("1000", "1111");
  EXPECT_EQ(result.sum, "0111");
  EXPECT_TRUE(result.overflow);
}

TEST(BinarySumTest, NegativeOverflow2) {
  // 4-bit: -5 + -4 = -9 (overflow)
  // 1011 + 1100 = 0111
  auto result = binarySum("1011", "1100");
  EXPECT_EQ(result.sum, "0111");
  EXPECT_TRUE(result.overflow);
}

// No overflow with negatives
TEST(BinarySumTest, NegativePlusPositiveNoOverflow) {
  // 4-bit: -1 + 1 = 0
  // 1111 + 0001 = 0000
  auto result = binarySum("1111", "0001");
  EXPECT_EQ(result.sum, "0000");
  EXPECT_FALSE(result.overflow);
}

TEST(BinarySumTest, NegativePlusPositiveNoOverflow2) {
  // 4-bit: -3 + 5 = 2
  // 1101 + 0101 = 0010
  auto result = binarySum("1101", "0101");
  EXPECT_EQ(result.sum, "0010");
  EXPECT_FALSE(result.overflow);
}

TEST(BinarySumTest, NegativePlusNegativeNoOverflow) {
  // 4-bit: -1 + -2 = -3
  // 1111 + 1110 = 1101
  auto result = binarySum("1111", "1110");
  EXPECT_EQ(result.sum, "1101");
  EXPECT_FALSE(result.overflow);
}

// Max values
TEST(BinarySumTest, MaxPositive) {
  // 4-bit: 7 + 0 = 7
  auto result = binarySum("0111", "0000");
  EXPECT_EQ(result.sum, "0111");
  EXPECT_FALSE(result.overflow);
}

TEST(BinarySumTest, MinNegative) {
  // 4-bit: -8 + 0 = -8
  auto result = binarySum("1000", "0000");
  EXPECT_EQ(result.sum, "1000");
  EXPECT_FALSE(result.overflow);
}

// 8-bit tests
TEST(BinarySumTest, EightBitNoOverflow) {
  // 100 + 27 = 127
  // 01100100 + 00011011 = 01111111
  auto result = binarySum("01100100", "00011011");
  EXPECT_EQ(result.sum, "01111111");
  EXPECT_FALSE(result.overflow);
}

TEST(BinarySumTest, EightBitOverflow) {
  // 127 + 1 = 128 (overflow in 8-bit signed)
  // 01111111 + 00000001 = 10000000
  auto result = binarySum("01111111", "00000001");
  EXPECT_EQ(result.sum, "10000000");
  EXPECT_TRUE(result.overflow);
}

} // namespace SCO