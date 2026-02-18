
#include "gtest/gtest.h"

#include "chapter5.6.2_towers.hpp"

namespace SCO {
TEST(TowersTest, MoveOneDisk) {
  int moveCount = towers(1, 1, 3);
  EXPECT_EQ(moveCount, 1);
}

TEST(TowersTest, MoveTwoDisks) {
  int moveCount = towers(2, 1, 3);
  EXPECT_EQ(moveCount, 3);
}

TEST(TowersTest, MoveThreeDisks) {
  int moveCount = towers(3, 1, 3);
  EXPECT_EQ(moveCount, 7);
}

TEST(TowersTest, MoveFourDisks) {
  int moveCount = towers(4, 1, 3);
  EXPECT_EQ(moveCount, 15);
}

TEST(TowersTest, MoveFiveDisks) {
  int moveCount = towers(5, 1, 3);
  EXPECT_EQ(moveCount, 31);
}
} // namespace SCO