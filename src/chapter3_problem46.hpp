#pragma once

#include <array>
#include <concepts>
#include <ostream>
#include <print>
#include <variant>
#include <vector>

namespace SCO {
template <typename... Args>
  requires(std::same_as<Args, bool> and ...)
bool nand(Args... args) {
  return not(args and ...);
}

struct InputPin {
  std::size_t index;
};

struct OutputPin {
  std::size_t index;
};

struct GatePos {
  std::size_t row;
  std::size_t col;
};

struct GateOutput {
  GatePos gatePos;
};

enum class GateInputName { InputA = 0, InputB };

struct GateInput {
  GatePos gatePos;
  GateInputName gateInputName;
};

enum class SignalSourceType { Unused = 0, InputPin, GateOutput };

enum class SignalDestinationType { Unused = 0, OutputPin, GateInput };

struct SignalSource {
  SignalSourceType type;
  std::variant<InputPin, GateOutput> source;
};

struct SignalDestination {
  SignalDestinationType type;
  std::variant<OutputPin, GateInput> destination;
};

struct WireConnection {
  SignalSource signalSource;
  SignalDestination signalDestination;
};

struct NANGGateInputMapping {
  std::array<std::size_t, 2> inputs;
};

struct NANDGateArrayResult {
  std::vector<bool> inputs;
  std::vector<bool> outputs;
};

std::ostream &
operator<<(std::ostream &os,
           const std::vector<NANDGateArrayResult> &nandGateArrayResult);

std::vector<NANDGateArrayResult>
simulateGateArray(std::size_t numRows, std::size_t numCols,
                  std::size_t numInputs, std::size_t numOutputs,
                  std::vector<WireConnection> wireConnections);
} // namespace SCO
