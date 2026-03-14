#include <iostream>
#include <print>

#include "chapter6_problem38.hpp"

namespace SCO {
AccessResult LRUStrategy::load(int pageIndex, PagedVM &pagedVM) {
  AccessResult result;

  if (pagedVM.hit(pageIndex)) {
    // std::print("[LRU] Page {} hit\n", pageIndex);
    result.hit = true;
    pages.erase(freeMap[pageIndex]);
    pages.push_front(pageIndex);
    freeMap[pageIndex] = pages.begin();
    pagedVM.pages[pageIndex].valid = true;
  } else if (pages.size() < pagedVM.numPhysicalPages) {
    // std::print("[LRU] Page {} fault (no eviction)\n", pageIndex);
    pages.push_front(pageIndex);
    freeMap[pageIndex] = pages.begin();
    pagedVM.pages[pageIndex].valid = true;
  } else { // Evict
    // std::print("[LRU] Page {} fault (evicting page {})\n", pageIndex,
    //            pages.back());
    result.evict = true;
    int evictedPageIndex = pages.back();
    freeMap.erase(evictedPageIndex);
    pages.pop_back();
    pages.push_front(pageIndex);
    freeMap[pageIndex] = pages.begin();
    pagedVM.pages[evictedPageIndex].valid = false;
    pagedVM.pages[pageIndex].valid = true;
  }

  return result;
}

AccessResult FIFOStrategy::load(int pageIndex, PagedVM &pagedVM) {
  AccessResult result;

  if (pagedVM.hit(pageIndex)) {
    // std::print("[FIFO] Page {} hit\n", pageIndex);
    result.hit = true;
    pagedVM.pages[pageIndex].valid = true;
  } else if (pages.size() < pagedVM.numPhysicalPages) {
    // std::print("[FIFO] Page {} fault (no eviction)\n", pageIndex);
    pages.push(pageIndex);
    pagedVM.pages[pageIndex].valid = true;
  } else { // Evict
    // std::print("[FIFO] Page {} fault (evicting page {})\n", pageIndex,
    //            pages.back());
    result.evict = true;
    int evictedPageIndex = pages.front();
    pages.pop();
    pages.push(pageIndex);
    pagedVM.pages[evictedPageIndex].valid = false;
    pagedVM.pages[pageIndex].valid = true;
  }

  return result;
}

AccessResult PagedVM::access(int address) {
  int pageIndex = getPageIndex(address);
  AccessResult result = strategy->load(pageIndex, *this);

  if (result.hit) {
    statistics.hits++;
  } else {
    statistics.faults++;
    if (result.evict) {
      statistics.evicts++;
    }
  }

  return result;
}
} // namespace SCO
