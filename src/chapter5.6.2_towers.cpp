#include <iostream>
#include <print>

#include "chapter5.6.2_towers.hpp"

namespace SCO {
// Towers of Hanoi recursive solution
// numberOfDisks: number of disks to move
// source: the peg to move from (1, 2, or 3)
// destination: the peg to move to (1, 2, or 3)
int towers(int numberOfDisks, int source, int destination) {
  static constexpr bool debug = true;
  if (numberOfDisks == 1) {
    if (debug) {
      std::print("Moving disk from {} to {}\n", source, destination);
    }
    return 1;
  }

  int moveCount = 0;
  int k = 6 - source - destination;
  // Move to intermediate position
  moveCount += towers(numberOfDisks - 1, source, k);
  // Move largest disk to destination
  moveCount += towers(1, source, destination);
  // Move from intermediate to destination
  moveCount += towers(numberOfDisks - 1, k, destination);

  return moveCount;
}
} // namespace SCO
