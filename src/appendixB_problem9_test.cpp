#include "appendixB_problem9.hpp"
#include "gtest/gtest.h"

namespace SCO {
TEST(AppendixBProblem9, TestSum) {
  EXPECT_FLOAT_EQ(sum(1.5f, 2.5f), 4.0f);
  EXPECT_FLOAT_EQ(sum(-1.5f, -2.5f), -4.0f);
  EXPECT_FLOAT_EQ(sum(1.5f, -2.5f), -1.0f);
  EXPECT_FLOAT_EQ(sum(-1.5f, 2.5f), 1.0f);
  EXPECT_FLOAT_EQ(sum(0.0f, 0.0f), 0.0f);
  EXPECT_FLOAT_EQ(sum(1.0f, 0.0f), 1.0f);
  EXPECT_FLOAT_EQ(sum(0.0f, 1.0f), 1.0f);
  EXPECT_FLOAT_EQ(sum(-1.0f, 0.0f), -1.0f);
  EXPECT_FLOAT_EQ(sum(0.0f, -1.0f), -1.0f);
}
} // namespace SCO