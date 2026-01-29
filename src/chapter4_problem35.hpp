#pragma once

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace SCO {
struct CacheLine {
  bool valid = false;
  std::uint32_t tag = 0;
  std::vector<std::byte> data;

  CacheLine(std::size_t size) : data(size) {}
  std::size_t size() const { return data.size(); }
};

struct CacheAccessResult {
  bool hit = false;
};

struct CacheStatistics {
  std::size_t totalAccesses = 0;
  std::size_t hits = 0;
  std::size_t misses = 0;

  double getHitRate() {
    return totalAccesses == 0 ? 0.0 : static_cast<double>(hits) / totalAccesses;
  }
};

struct DirectMappedCache {
  std::vector<CacheLine> cacheLines;
  CacheStatistics statistics;
  std::size_t lineIndexSize;
  std::size_t offsetSize;
  std::size_t lineSize;
  std::size_t tagSize;

  DirectMappedCache(std::size_t numEntries, std::size_t lineSize)
      : cacheLines(numEntries, CacheLine{lineSize}), lineSize(lineSize),
        lineIndexSize(std::bit_width(numEntries - 1)),
        offsetSize(std::bit_width(lineSize - 1)),
        tagSize(sizeof(std::uint32_t) * 8 - (lineIndexSize + offsetSize)) {}

  std::size_t getNumEntries() const { return cacheLines.size(); }
  std::size_t getLineSize() const { return lineSize; }
  CacheStatistics getStatistics() { return statistics; }
  std::uint32_t getOffset(std::uint32_t address) {
    std::uint32_t offsetMask = (1u << offsetSize) - 1;
    return address & offsetMask;
  }
  std::uint32_t getTag(std::uint32_t address) {
    return address >> (offsetSize + lineIndexSize);
  }
  std::uint32_t getLine(std::uint32_t address) {
    std::uint32_t lineMask = (1u << lineIndexSize) - 1;
    return (address >> offsetSize) & lineMask;
  }
  void reset();
  CacheAccessResult access(std::uint32_t address);
};
} // namespace SCO
