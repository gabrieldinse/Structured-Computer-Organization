
#include "gtest/gtest.h"

#include "chapter5.6.2_towers.hpp"

namespace SCO {
TEST(TowersTest, MoveOneDisk) {
  testing::internal::CaptureStdout();
  towers(1, 1, 3);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "Moving disk from 1 to 3\n");
  std::print("Captured output: {}", output); // For debugging
}

TEST(TowersTest, MoveTwoDisks) {
  testing::internal::CaptureStdout();
  towers(2, 1, 3);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(
      output,
      "Moving disk from 1 to 2\nMoving disk from 1 to 3\nMoving disk from 2 to "
      "3\n");
  std::print("Captured output: {}", output); // For debugging
}

TEST(TowersTest, MoveThreeDisks) {
  testing::internal::CaptureStdout();
  towers(3, 1, 3);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(
      output,
      "Moving disk from 1 to 3\nMoving disk from 1 to 2\nMoving disk from 3 to "
      "2\nMoving disk from 1 to 3\nMoving disk from 2 to 1\nMoving disk from "
      "2 to 3\nMoving disk from 1 to 3\n");
  std::print("Captured output: {}", output); // For debugging
}
} // namespace SCO