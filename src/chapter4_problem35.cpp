#include "chapter4_problem35.hpp"

namespace SCO {

void DirectMappedCache::reset() {
  statistics.hits = 0;
  statistics.misses = 0;
  statistics.totalAccesses = 0;
  for (auto &cacheLine : cacheLines) {
    cacheLine.valid = false;
  }
}

CacheAccessResult DirectMappedCache::access(std::uint32_t address) {
  CacheAccessResult result;
  auto line = getLine(address);
  auto tag = getTag(address);
  auto &cacheLine = cacheLines[line];

  if (cacheLine.valid and (tag == cacheLine.tag)) {
    result.hit = true;
    statistics.hits++;
  } else {
    statistics.misses++;
    cacheLine.tag = tag;
    cacheLine.valid = true;
  }
  statistics.totalAccesses++;

  return result;
}
} // namespace SCO
