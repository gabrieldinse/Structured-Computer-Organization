#pragma once

#include <print>

namespace SCO {
// Towers of Hanoi recursive solution
// numberOfDisks: number of disks to move
// source: the peg to move from (1, 2, or 3)
// destination: the peg to move to (1, 2, or 3)
int towers(int numberOfDisks, int source, int destination);
} // namespace SCO
