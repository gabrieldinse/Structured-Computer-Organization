#pragma once

#include <list>
#include <memory>
#include <print>
#include <queue>
#include <unordered_map>
#include <vector>

namespace SCO {

struct TableEntry {
  bool valid = false;

  // Unused. For future use.
  bool dirty = false;
  int address = 0;
};

using PageTable = std::vector<TableEntry>;

struct AccessResult {
  bool hit = false;
  bool evict = false;
};

struct PagedVM;
struct EvictStrategy {
  virtual ~EvictStrategy(){};
  virtual AccessResult load(int page, PagedVM &pagedVM) = 0;
  virtual void reset() = 0;
};

struct LRUStrategy : public EvictStrategy {
  std::list<int> pages;
  std::unordered_map<int, std::list<int>::iterator> freeMap;

  virtual AccessResult load(int page, PagedVM &pagedVM) override;
  virtual void reset() override {
    pages.clear();
    freeMap.clear();
  }
};

struct FIFOStrategy : public EvictStrategy {
  std::queue<int> pages;

  virtual AccessResult load(int page, PagedVM &pagedVM) override;
  virtual void reset() override {
    while (not pages.empty()) {
      pages.pop();
    }
  }
};

enum class Strategy {
  LRU = 0,
  FIFO,
};

struct VMStatistics {
  int hits = 0;
  int faults = 0;
  int evicts = 0;
};

struct PagedVM {
  PageTable pages;
  int pageSize;
  int numPhysicalPages;
  std::unique_ptr<EvictStrategy> strategy;
  VMStatistics statistics;

  PagedVM(int numPhysicalPages, int numVirtualPages, int pageSize,
          Strategy strategy = Strategy::LRU)
      : pageSize(pageSize), numPhysicalPages(numPhysicalPages) {
    pages.resize(numVirtualPages);
    switch (strategy) {
    case Strategy::LRU:
      this->strategy = std::make_unique<LRUStrategy>();
      break;
    default:
    case Strategy::FIFO:
      this->strategy = std::make_unique<FIFOStrategy>();
      break;
    }
  }

  void reset() {
    statistics = VMStatistics();
    for (auto &entry : pages) {
      entry.valid = false;
    }
    strategy->reset();
  }
  bool hit(int pageIndex) { return pages[pageIndex].valid; }

  int getPageIndex(int address) { return address / pageSize; }

  AccessResult access(int address);
};
} // namespace SCO
