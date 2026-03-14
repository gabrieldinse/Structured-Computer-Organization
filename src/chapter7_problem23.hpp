#pragma once

#include <algorithm>
#include <array>
#include <list>
#include <optional>
#include <string>

namespace SCO {
struct HashEntry {
  std::string symbol;
  int address;
};

template <std::size_t N> struct SymbolTable {
  std::array<std::list<HashEntry>, N> hashTable;

  void enter(const std::string &symbol, int address);
  std::optional<int> lookup(const std::string &symbol);

  std::size_t getHash(const std::string &input) {
    int sum = 0;
    for (char c : input) {
      sum += c;
    }
    return sum % hashTable.size();
  }
};

template <std::size_t N>
void SymbolTable<N>::enter(const std::string &symbol, int address) {
  auto &hashEntry = hashTable[getHash(symbol)];
  auto it = std::find_if(
      hashEntry.begin(), hashEntry.end(),
      [&](const HashEntry &entry) { return entry.symbol == symbol; });

  if (it == hashEntry.end()) {
    hashTable[getHash(symbol)].push_back(HashEntry{symbol, address});
  } else {
    it->address = address;
  }
}

template <std::size_t N>
std::optional<int> SymbolTable<N>::lookup(const std::string &symbol) {
  const auto &hashEntry = hashTable[getHash(symbol)];
  const auto it = std::find_if(
      hashEntry.begin(), hashEntry.end(),
      [&](const HashEntry &entry) { return entry.symbol == symbol; });

  if (it == hashEntry.end()) {
    return std::nullopt;
  }

  return it->address;
}
} // namespace SCO
