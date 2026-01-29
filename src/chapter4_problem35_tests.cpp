#include <print>
#include <vector>

#include "gtest/gtest.h"

#include "chapter4_problem35.hpp"

namespace SCO {

// Test fixture for direct mapped cache tests
class DirectMappedCacheTest : public ::testing::Test {
protected:
  void SetUp() override {}
};

// ============================================================================
// Basic Construction Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, ConstructionWithValidParameters) {
  // Cache with 8 entries, 16 bytes per line
  DirectMappedCache cache(8, 16);
  EXPECT_EQ(cache.getNumEntries(), 8);
  EXPECT_EQ(cache.getLineSize(), 16);
}

TEST_F(DirectMappedCacheTest, ConstructionPowerOfTwoEntries) {
  // Various power-of-two sizes
  DirectMappedCache cache1(1, 8);
  EXPECT_EQ(cache1.getNumEntries(), 1);

  DirectMappedCache cache4(4, 8);
  EXPECT_EQ(cache4.getNumEntries(), 4);

  DirectMappedCache cache256(256, 32);
  EXPECT_EQ(cache256.getNumEntries(), 256);
}

TEST_F(DirectMappedCacheTest, ConstructionVariousLineSizes) {
  DirectMappedCache cache8(16, 8);
  EXPECT_EQ(cache8.getLineSize(), 8);

  DirectMappedCache cache32(16, 32);
  EXPECT_EQ(cache32.getLineSize(), 32);

  DirectMappedCache cache64(16, 64);
  EXPECT_EQ(cache64.getLineSize(), 64);
}

// ============================================================================
// Cache Miss Tests (Empty Cache)
// ============================================================================

TEST_F(DirectMappedCacheTest, FirstAccessIsAlwaysMiss) {
  DirectMappedCache cache(8, 16);
  auto result = cache.access(0x0000);
  EXPECT_FALSE(result.hit);
}

TEST_F(DirectMappedCacheTest, MultipleFirstAccessesAreMisses) {
  DirectMappedCache cache(8, 16);

  // Different addresses should all miss on first access
  EXPECT_FALSE(cache.access(0x0000).hit);
  EXPECT_FALSE(cache.access(0x1000).hit);
  EXPECT_FALSE(cache.access(0x2000).hit);
  EXPECT_FALSE(cache.access(0x3000).hit);
}

// ============================================================================
// Cache Hit Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, SecondAccessToSameAddressIsHit) {
  DirectMappedCache cache(8, 16);

  cache.access(0x0000);               // Miss - loads into cache
  auto result = cache.access(0x0000); // Should be hit
  EXPECT_TRUE(result.hit);
}

TEST_F(DirectMappedCacheTest, AccessWithinSameLineIsHit) {
  // Line size = 16 bytes, so addresses 0x00-0x0F are in same line
  DirectMappedCache cache(8, 16);

  cache.access(0x0000); // Miss - loads line containing 0x00-0x0F
  EXPECT_TRUE(cache.access(0x0001).hit);
  EXPECT_TRUE(cache.access(0x0005).hit);
  EXPECT_TRUE(cache.access(0x000F).hit);
}

TEST_F(DirectMappedCacheTest, AccessToDifferentLineIsMiss) {
  // Line size = 16 bytes
  DirectMappedCache cache(8, 16);

  cache.access(0x0000); // Loads line 0
  // Address 0x0010 is in a different line
  EXPECT_FALSE(cache.access(0x0010).hit);
}

// ============================================================================
// Index Calculation Tests (Direct Mapping)
// ============================================================================

TEST_F(DirectMappedCacheTest, SameIndexDifferentTagCausesMiss) {
  // 8 entries, 16-byte lines
  // Index = (address / lineSize) % numEntries
  // Addresses 0x0000 and 0x0080 map to same index (0)
  DirectMappedCache cache(8, 16);

  // The address is split into:
  // Offset: lower 4 bits (16 bytes line size)
  // Index: next 3 bits (8 entries)
  // Tag: remaining upper bits

  // Offset: 0x0000 -> 0000
  // Index: 0x0000 -> 000
  // Tag: 0x0000 -> 0000...
  cache.access(0x0000); // Index 0, some tag

  // Offset: 0x0080 -> 0000 (lower 4 bits)
  // Index: 0x0080 -> 000 (next 3 bits)
  // Tag: 0x0080 -> 0000...1
  cache.access(0x0080); // Index 1, different tag - evicts previous
  EXPECT_FALSE(cache.access(0x0000).hit); // Should miss (was evicted)

  // Binary: 000010000
  EXPECT_FALSE(cache.access(0x0010).hit); // Should miss
  // Binary: 001010000
  EXPECT_FALSE(
      cache.access(0x0090).hit); // Should miss again (same index as 0x0010)
}

TEST_F(DirectMappedCacheTest, DifferentIndexesCoexist) {
  // 8 entries, 16-byte lines
  DirectMappedCache cache(8, 16);

  cache.access(0x0000); // Index 0
  cache.access(0x0010); // Index 1
  cache.access(0x0020); // Index 2

  // All should still be in cache
  EXPECT_TRUE(cache.access(0x0000).hit);
  EXPECT_TRUE(cache.access(0x0010).hit);
  EXPECT_TRUE(cache.access(0x0020).hit);
}

// ============================================================================
// Statistics Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, StatisticsInitiallyZero) {
  DirectMappedCache cache(8, 16);
  auto stats = cache.getStatistics();

  EXPECT_EQ(stats.totalAccesses, 0);
  EXPECT_EQ(stats.hits, 0);
  EXPECT_EQ(stats.misses, 0);
}

TEST_F(DirectMappedCacheTest, StatisticsCountAccesses) {
  DirectMappedCache cache(8, 16);

  cache.access(0x0000);
  cache.access(0x0000);
  cache.access(0x1000);

  auto stats = cache.getStatistics();
  EXPECT_EQ(stats.totalAccesses, 3);
}

TEST_F(DirectMappedCacheTest, StatisticsCountHitsAndMisses) {
  DirectMappedCache cache(8, 16);

  cache.access(0x0000); // Miss
  cache.access(0x0000); // Hit
  cache.access(0x0000); // Hit
  cache.access(0x1000); // Miss

  auto stats = cache.getStatistics();
  EXPECT_EQ(stats.hits, 2);
  EXPECT_EQ(stats.misses, 2);
}

TEST_F(DirectMappedCacheTest, HitRateCalculation) {
  DirectMappedCache cache(8, 16);

  cache.access(0x0000); // Miss
  cache.access(0x0000); // Hit
  cache.access(0x0000); // Hit
  cache.access(0x0000); // Hit

  auto stats = cache.getStatistics();
  EXPECT_DOUBLE_EQ(stats.getHitRate(), 0.75); // 3 hits / 4 accesses
}

TEST_F(DirectMappedCacheTest, HitRateZeroWhenNoAccesses) {
  DirectMappedCache cache(8, 16);
  auto stats = cache.getStatistics();
  EXPECT_DOUBLE_EQ(stats.getHitRate(), 0.0);
}

// ============================================================================
// Reset Functionality Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, ResetClearsCache) {
  DirectMappedCache cache(8, 16);

  cache.access(0x0000);
  EXPECT_TRUE(cache.access(0x0000).hit);

  cache.reset();

  // After reset, same address should miss
  EXPECT_FALSE(cache.access(0x0000).hit);
}

TEST_F(DirectMappedCacheTest, ResetClearsStatistics) {
  DirectMappedCache cache(8, 16);

  cache.access(0x0000);
  cache.access(0x0000);
  cache.reset();

  auto stats = cache.getStatistics();
  EXPECT_EQ(stats.totalAccesses, 0);
  EXPECT_EQ(stats.hits, 0);
  EXPECT_EQ(stats.misses, 0);
}

// ============================================================================
// Sequential Access Pattern Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, SequentialAccessWithinLine) {
  // 8 entries, 32-byte lines
  DirectMappedCache cache(8, 32);

  // Sequential access within one line (addresses 0-31)
  cache.access(0x0000); // Miss
  for (uint32_t addr = 1; addr < 32; ++addr) {
    EXPECT_TRUE(cache.access(addr).hit) << "Address " << addr << " should hit";
  }
}

TEST_F(DirectMappedCacheTest, SequentialAccessAcrossLines) {
  // 8 entries, 16-byte lines
  DirectMappedCache cache(8, 16);

  // Access first byte of consecutive lines
  for (uint32_t line = 0; line < 8; ++line) {
    uint32_t addr = line * 16;
    auto result = cache.access(addr);
    EXPECT_FALSE(result.hit)
        << "First access to line " << line << " should miss";
  }

  // All 8 lines should now be in cache
  for (uint32_t line = 0; line < 8; ++line) {
    uint32_t addr = line * 16;
    EXPECT_TRUE(cache.access(addr).hit) << "Line " << line << " should hit";
  }
}

// ============================================================================
// Strided Access Pattern Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, StridedAccessCausingConflicts) {
  // 4 entries, 16-byte lines
  // Stride that causes same-index conflicts
  DirectMappedCache cache(4, 16);

  // Stride = 4 * 16 = 64 bytes maps to same index
  cache.access(0x0000); // Index 0
  cache.access(0x0040); // Index 0 (conflict!)
  cache.access(0x0000); // Should miss (evicted)

  auto stats = cache.getStatistics();
  EXPECT_EQ(stats.misses, 3); // All three should miss
}

TEST_F(DirectMappedCacheTest, StridedAccessNoConflicts) {
  // 8 entries, 16-byte lines
  DirectMappedCache cache(8, 16);

  // Stride = 16 bytes, accesses different indices
  std::vector<uint32_t> addresses = {0x00, 0x10, 0x20, 0x30,
                                     0x40, 0x50, 0x60, 0x70};

  for (auto addr : addresses) {
    cache.access(addr);
  }

  // All should hit on second pass
  for (auto addr : addresses) {
    EXPECT_TRUE(cache.access(addr).hit);
  }
}

// ============================================================================
// Thrashing Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, ThrashingWithMoreLinesThanEntries) {
  // 4 entries, 16-byte lines
  DirectMappedCache cache(4, 16);

  // Access 5 different lines that map to index 0
  // Lines at addresses: 0, 64, 128, 192, 256 all map to index 0
  std::vector<uint32_t> addresses = {0, 64, 128, 192, 256};

  // First pass: all misses
  for (auto addr : addresses) {
    EXPECT_FALSE(cache.access(addr).hit);
  }

  // Second pass: still all misses (thrashing)
  for (auto addr : addresses) {
    EXPECT_FALSE(cache.access(addr).hit);
  }

  auto stats = cache.getStatistics();
  EXPECT_EQ(stats.hits, 0);
  EXPECT_EQ(stats.misses, 10);
}

// ============================================================================
// Large Address Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, LargeAddresses) {
  DirectMappedCache cache(256, 64);

  uint32_t largeAddr = 0x10000000;
  cache.access(largeAddr);
  EXPECT_TRUE(cache.access(largeAddr).hit);
  EXPECT_TRUE(cache.access(largeAddr + 1).hit);
  EXPECT_TRUE(cache.access(largeAddr + 63).hit);
}

// ============================================================================
// Edge Cases
// ============================================================================

TEST_F(DirectMappedCacheTest, SingleEntryCacheConstantMisses) {
  // Single entry cache - every unique line access evicts the previous
  DirectMappedCache cache(1, 16);

  cache.access(0x0000);
  cache.access(0x0010); // Different line, evicts previous

  EXPECT_FALSE(cache.access(0x0000).hit);
}

TEST_F(DirectMappedCacheTest, SingleByteLineSize) {
  // Minimum line size
  DirectMappedCache cache(8, 1);

  cache.access(0x0000);
  EXPECT_TRUE(cache.access(0x0000).hit);
  EXPECT_FALSE(cache.access(0x0001).hit); // Different line
}

TEST_F(DirectMappedCacheTest, AddressZero) {
  DirectMappedCache cache(8, 16);

  cache.access(0x0000);
  EXPECT_TRUE(cache.access(0x0000).hit);
}

// ============================================================================
// Realistic Workload Simulation Tests
// ============================================================================

TEST_F(DirectMappedCacheTest, ArrayTraversal) {
  // Simulating traversal of an array with 4-byte elements
  DirectMappedCache cache(16, 32); // 16 entries, 32-byte lines

  // Traverse an array of 64 elements (256 bytes = 8 cache lines)
  for (uint32_t i = 0; i < 64; ++i) {
    cache.access(i * 4); // 4-byte stride
  }

  auto stats = cache.getStatistics();
  // With 32-byte lines and 4-byte stride, we get 8 elements per line
  // So we expect 8 compulsory misses (one per line)
  EXPECT_EQ(stats.misses, 8);
  EXPECT_EQ(stats.hits, 56); // 64 - 8 = 56 hits
}

TEST_F(DirectMappedCacheTest, MatrixColumnTraversal) {
  // Column traversal typically has poor cache performance
  // Simulating 8x8 matrix with 4-byte elements, row-major layout
  // Row stride = 8 * 4 = 32 bytes
  DirectMappedCache cache(8, 16);

  // Traverse column 0
  for (uint32_t row = 0; row < 8; ++row) {
    cache.access(row * 32); // Column 0 of each row
  }

  auto stats = cache.getStatistics();
  // Each access is to a different cache line, so all misses
  EXPECT_EQ(stats.misses, 8);
}

TEST_F(DirectMappedCacheTest, LoopWithTemporalLocality) {
  DirectMappedCache cache(8, 16);

  // Repeatedly access same small set of addresses
  for (int iteration = 0; iteration < 10; ++iteration) {
    cache.access(0x0000);
    cache.access(0x0004);
    cache.access(0x0008);
  }

  auto stats = cache.getStatistics();
  // First iteration: 1 miss (all within same line)
  // Remaining 29 accesses: all hits
  EXPECT_EQ(stats.misses, 1);
  EXPECT_EQ(stats.hits, 29);
}

// ============================================================================
// Parameter Variation Tests (for experimentation)
// ============================================================================

TEST_F(DirectMappedCacheTest, VaryingCacheSizeImpact) {
  // Test same access pattern with different cache sizes
  std::vector<uint32_t> accessPattern;
  for (uint32_t i = 0; i < 32; ++i) {
    accessPattern.push_back(i * 16); // Access 32 different lines
  }

  // Small cache: 8 entries
  DirectMappedCache smallCache(8, 16);
  for (auto addr : accessPattern)
    smallCache.access(addr);
  for (auto addr : accessPattern)
    smallCache.access(addr);
  auto smallStats = smallCache.getStatistics();

  // Large cache: 64 entries
  DirectMappedCache largeCache(64, 16);
  for (auto addr : accessPattern)
    largeCache.access(addr);
  for (auto addr : accessPattern)
    largeCache.access(addr);
  auto largeStats = largeCache.getStatistics();

  // Larger cache should have better hit rate for this pattern
  EXPECT_GT(largeStats.getHitRate(), smallStats.getHitRate());
}

TEST_F(DirectMappedCacheTest, VaryingLineSizeImpact) {
  // Sequential access benefits from larger line sizes
  std::vector<uint32_t> sequentialAccess;
  for (uint32_t i = 0; i < 128; ++i) {
    sequentialAccess.push_back(i); // Byte-by-byte access
  }

  // Small line size: 8 bytes
  DirectMappedCache smallLineCache(16, 8);
  for (auto addr : sequentialAccess)
    smallLineCache.access(addr);
  auto smallLineStats = smallLineCache.getStatistics();

  // Large line size: 64 bytes
  DirectMappedCache largeLineCache(16, 64);
  for (auto addr : sequentialAccess)
    largeLineCache.access(addr);
  auto largeLineStats = largeLineCache.getStatistics();

  // Larger lines should have better hit rate for sequential access
  EXPECT_GT(largeLineStats.getHitRate(), smallLineStats.getHitRate());
}

} // namespace SCO