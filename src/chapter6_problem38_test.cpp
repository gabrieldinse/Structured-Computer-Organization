
#include "gtest/gtest.h"

#include "chapter6_problem38.hpp"

namespace SCO {
TEST(VMTest, SimpleLRUAccess) {
  PagedVM pagedVM(2, 4, 1024, Strategy::LRU);
  AccessResult result;

  // Access page 0
  result = pagedVM.access(0);
  EXPECT_FALSE(result.hit);
  EXPECT_FALSE(result.evict);

  // Access page 1
  result = pagedVM.access(1024);
  EXPECT_FALSE(result.hit);
  EXPECT_FALSE(result.evict);

  // Access page 0 again (should be a hit)
  result = pagedVM.access(0);
  EXPECT_TRUE(result.hit);
  EXPECT_FALSE(result.evict);

  // Access page 2 (should cause eviction of page 1)
  result = pagedVM.access(2048);
  EXPECT_FALSE(result.hit);
  EXPECT_TRUE(result.evict);

  // Access page 1 again (should be a miss and cause eviction of page 0)
  result = pagedVM.access(1024);
  EXPECT_FALSE(result.hit);
  EXPECT_TRUE(result.evict);

  EXPECT_EQ(pagedVM.statistics.hits, 1);
  EXPECT_EQ(pagedVM.statistics.faults, 4);
  EXPECT_EQ(pagedVM.statistics.evicts, 2);
  std::print("LRU Stats: Hits={}, Faults={}, Evicts={}\n",
             pagedVM.statistics.hits, pagedVM.statistics.faults,
             pagedVM.statistics.evicts);
}

TEST(VMTest, SimpleFIFOAccess) {
  PagedVM pagedVM(2, 4, 1024, Strategy::FIFO);
  AccessResult result;

  // Access page 0
  result = pagedVM.access(0);
  EXPECT_FALSE(result.hit);
  EXPECT_FALSE(result.evict);

  // Access page 1
  result = pagedVM.access(1024);
  EXPECT_FALSE(result.hit);
  EXPECT_FALSE(result.evict);

  // Access page 0 again (should be a hit)
  result = pagedVM.access(0);
  EXPECT_TRUE(result.hit);
  EXPECT_FALSE(result.evict);

  // Access page 2 (should cause eviction of page 0)
  result = pagedVM.access(2048);
  EXPECT_FALSE(result.hit);
  EXPECT_TRUE(result.evict);

  // Access page 1 again (should be a hit)
  result = pagedVM.access(1024);
  EXPECT_TRUE(result.hit);
  EXPECT_FALSE(result.evict);

  EXPECT_EQ(pagedVM.statistics.hits, 2);
  EXPECT_EQ(pagedVM.statistics.faults, 3);
  EXPECT_EQ(pagedVM.statistics.evicts, 1);
  std::print("FIFO Stats: Hits={}, Faults={}, Evicts={}\n",
             pagedVM.statistics.hits, pagedVM.statistics.faults,
             pagedVM.statistics.evicts);
}

TEST(VMTest, LRUAndFIFOAccessVeryBigNumberOfRandomPages) {
  PagedVM lruPagedVM(100, 1000, 1024, Strategy::LRU);
  PagedVM fifoPagedVM(100, 1000, 1024, Strategy::FIFO);
  AccessResult result;

  for (int i = 0; i < 10000; i++) {
    int address = rand() % (1000 * 1024);
    lruPagedVM.access(address);
    fifoPagedVM.access(address);
  }

  EXPECT_EQ(lruPagedVM.statistics.hits + lruPagedVM.statistics.faults, 10000);
  EXPECT_EQ(fifoPagedVM.statistics.hits + fifoPagedVM.statistics.faults, 10000);

  std::print("\nLRU Stats: Hits={}, Faults={}, Evicts={}\n",
             lruPagedVM.statistics.hits, lruPagedVM.statistics.faults,
             lruPagedVM.statistics.evicts);
  std::print("\nFIFO Stats: Hits={}, Faults={}, Evicts={}\n\n",
             fifoPagedVM.statistics.hits, fifoPagedVM.statistics.faults,
             fifoPagedVM.statistics.evicts);
}

TEST(VMTest, LRUAndFIFOAccessWithConsecutivePages) {
  PagedVM lruPagedVM(100, 1000, 1024, Strategy::LRU);
  PagedVM fifoPagedVM(100, 1000, 1024, Strategy::FIFO);
  AccessResult result;
  for (int i = 0; i < 70000; i++) {
    int address;
    address = i * 4;
    lruPagedVM.access(address);
    fifoPagedVM.access(address);
  }

  for (int i = 0; i < 30000; i++) {
    int address = rand() % (1000 * 1024); // Random access
    lruPagedVM.access(address);
    fifoPagedVM.access(address);
  }

  EXPECT_EQ(lruPagedVM.statistics.hits + lruPagedVM.statistics.faults, 100000);
  EXPECT_EQ(fifoPagedVM.statistics.hits + fifoPagedVM.statistics.faults,
            100000);

  std::print("70% contiguous addresses:\n");
  std::print("\nLRU Stats: Hits={}, Faults={}, Evicts={}\n",
             lruPagedVM.statistics.hits, lruPagedVM.statistics.faults,
             lruPagedVM.statistics.evicts);
  std::print("\nFIFO Stats: Hits={}, Faults={}, Evicts={}\n\n",
             fifoPagedVM.statistics.hits, fifoPagedVM.statistics.faults,
             fifoPagedVM.statistics.evicts);

  lruPagedVM.reset();
  fifoPagedVM.reset();
  for (int i = 0; i < 100000; i++) {
    int address;
    address = i * 4;
    lruPagedVM.access(address);
    fifoPagedVM.access(address);
  }

  EXPECT_EQ(lruPagedVM.statistics.hits + lruPagedVM.statistics.faults, 100000);
  EXPECT_EQ(fifoPagedVM.statistics.hits + fifoPagedVM.statistics.faults,
            100000);

  std::print("100% contiguous addresses:\n");
  std::print("\nLRU Stats: Hits={}, Faults={}, Evicts={}\n",
             lruPagedVM.statistics.hits, lruPagedVM.statistics.faults,
             lruPagedVM.statistics.evicts);
  std::print("\nFIFO Stats: Hits={}, Faults={}, Evicts={}\n\n",
             fifoPagedVM.statistics.hits, fifoPagedVM.statistics.faults,
             fifoPagedVM.statistics.evicts);

  std::print("Now with 10-bytes spaces addresses\n");
  lruPagedVM.reset();
  fifoPagedVM.reset();
  for (int i = 0; i < 100000; i++) {
    int address;
    address = i * 10;
    lruPagedVM.access(address);
    fifoPagedVM.access(address);
  }

  EXPECT_EQ(lruPagedVM.statistics.hits + lruPagedVM.statistics.faults, 100000);
  EXPECT_EQ(fifoPagedVM.statistics.hits + fifoPagedVM.statistics.faults,
            100000);

  std::print("100% contiguous addresses:\n");
  std::print("\nLRU Stats: Hits={}, Faults={}, Evicts={}\n",
             lruPagedVM.statistics.hits, lruPagedVM.statistics.faults,
             lruPagedVM.statistics.evicts);
  std::print("\nFIFO Stats: Hits={}, Faults={}, Evicts={}\n\n",
             fifoPagedVM.statistics.hits, fifoPagedVM.statistics.faults,
             fifoPagedVM.statistics.evicts);
}
} // namespace SCO