
#include "gtest/gtest.h"

#include "chapter7_problem23.hpp"

namespace SCO {
TEST(SymbolTableTest, InsertAndLookup) {
  SymbolTable<10> table;
  // Insert symbols as in an assembler symbol table
  table.enter("START", 0);
  table.enter("LOOP", 10);
  table.enter("END", 100);

  // Lookup inserted symbols
  EXPECT_EQ(table.lookup("START"), 0);
  EXPECT_EQ(table.lookup("LOOP"), 10);
  EXPECT_EQ(table.lookup("END"), 100);
}

TEST(SymbolTableTest, LookupMissingSymbol) {
  SymbolTable<10> table;
  table.enter("FOO", 42);
  // Lookup a symbol that does not exist
  EXPECT_EQ(table.lookup("BAR"), std::nullopt);
}

TEST(SymbolTableTest, OverwriteSymbol) {
  SymbolTable<10> table;
  table.enter("LABEL", 5);
  table.enter("LABEL", 15); // Simulate redefinition (should shadow previous)
  EXPECT_EQ(table.lookup("LABEL"), 15);
}
} // namespace SCO