#include <print>

#include "gtest/gtest.h"

#include "chapter3_problem46.hpp"

namespace SCO {
class NANDGateArrayTest : public ::testing::Test {
protected:
  // Helper to create a WireConnection for InputPin -> GateInput
  WireConnection connectInputToGate(size_t pinIdx, size_t row, size_t col,
                                    GateInputName name) {
    return {{SignalSourceType::InputPin, InputPin{pinIdx}},
            {SignalDestinationType::GateInput, GateInput{{row, col}, name}}};
  }

  // Helper to create a WireConnection for GateOutput -> OutputPin
  WireConnection connectGateToOutput(size_t row, size_t col, size_t pinIdx) {
    return {{SignalSourceType::GateOutput, GateOutput{{row, col}}},
            {SignalDestinationType::OutputPin, OutputPin{pinIdx}}};
  }
};

TEST_F(NANDGateArrayTest, SimpleNotGate) {
  // Simple NOT gate (using the "unused inputs are 1" rule)
  // NAND(Input, 1) = NOT Input
  std::vector<WireConnection> wires = {
      connectInputToGate(0, 0, 0, GateInputName::InputA),
      connectGateToOutput(0, 0, 0)};

  auto results = simulateGateArray(1, 1, 1, 1, wires);
  std::cout << results << std::endl;

  ASSERT_EQ(results.size(), 2);
  // Input 0 -> Output NOT(0, 1) = 1
  EXPECT_EQ(results[0].inputs[0], false);
  EXPECT_EQ(results[0].outputs[0], true);

  // Input 1 -> Output NOT(1, 1) = 0
  EXPECT_EQ(results[1].inputs[0], true);
  EXPECT_EQ(results[1].outputs[0], false);
}

TEST_F(NANDGateArrayTest, AndGateConstruction) {
  std::vector<WireConnection> wires = {
      // Inputs to first gate
      connectInputToGate(0, 0, 0, GateInputName::InputA),
      connectInputToGate(1, 0, 0, GateInputName::InputB),
      // First gate output to second gate input
      {{SignalSourceType::GateOutput, GateOutput{{0, 0}}},
       {SignalDestinationType::GateInput,
        GateInput{{0, 1}, GateInputName::InputA}}},
      // Second gate output to final output
      connectGateToOutput(0, 1, 0)};

  auto results = simulateGateArray(1, 2, 2, 1, wires);
  std::cout << results << std::endl;

  // Truth table for AND: 00->0, 01->0, 10->0, 11->1
  bool expected_outputs[] = {false, false, false, true};
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(results[i].outputs[0], expected_outputs[i])
        << "Failed at index " << i;
  }
}

TEST_F(NANDGateArrayTest, UnconnectedOutputDefaultsToHigh) {
  std::vector<WireConnection> wires = {}; // No wires
  auto results = simulateGateArray(1, 1, 1, 1, wires);
  std::cout << results << std::endl;

  EXPECT_EQ(results[0].outputs[0], true);
  EXPECT_EQ(results[1].outputs[0], true);
}

TEST_F(NANDGateArrayTest, SRLatchFeedback) {
  std::vector<WireConnection> wires = {
      // Set (S) to Gate 0, Reset (R) to Gate 1
      // Note: NAND SR latches are active LOW
      connectInputToGate(0, 0, 0, GateInputName::InputA), // S_L
      connectInputToGate(1, 0, 1, GateInputName::InputB), // R_L

      // Cross-coupling
      {{SignalSourceType::GateOutput, GateOutput{{0, 0}}},
       {SignalDestinationType::GateInput,
        GateInput{{0, 1}, GateInputName::InputA}}},
      {{SignalSourceType::GateOutput, GateOutput{{0, 1}}},
       {SignalDestinationType::GateInput,
        GateInput{{0, 0}, GateInputName::InputB}}},

      connectGateToOutput(0, 0, 0) // Q
  };

  auto results = simulateGateArray(1, 2, 2, 1, wires);
  std::cout << results << std::endl;

  // If S=0, R=1 -> Q=1 (Set)
  // S is bit 0, R is bit 1. Index 1 is (S=1, R=0) in binary, so we check
  // specific indices.

  // R_L=0, S_L=1 (Index 1: bits 1, 0) -> Q should be 0
  EXPECT_EQ(results[1].outputs[0], false);

  // S_L=1, R_L=0 (Index 2: bits 0, 1) -> Q should be 1
  EXPECT_EQ(results[2].outputs[0], true);
}
} // namespace SCO