#pragma once

#include <array>
#include <concepts>
#include <cstdint>
#include <ostream>
#include <print>
#include <set>
#include <stack>
#include <unordered_map>
#include <variant>
#include <vector>

namespace SCO {
template <typename T> T getBit(T num, std::size_t pos) {
  return (num >> pos) & 1;
}

// Chapter 2 - Problem 35
std::int16_t hamming(char ascii);

// Chapter 2 - Problem 36
template <typename T> std::size_t hammingWeight(T val) {
  std::size_t bitsSet = 0;
  while (val) {
    bitsSet += val & 1;
    val >>= 1;
  }
  return bitsSet;
}

template <typename T>
std::size_t distance(const std::vector<T> &code, std::size_t codewordLength) {
  if (code.size() < 2) {
    return codewordLength;
  }

  std::size_t minDistance = codewordLength;
  for (std::size_t i = 0; i < code.size(); i++) {
    for (std::size_t j = i + 1; j < code.size(); j++) {
      T diff = code[i] ^ code[j];
      std::size_t bitsSet = hammingWeight(diff);
      if (bitsSet < minDistance) {
        minDistance = bitsSet;
      }

      if (minDistance == 1) {
        return 1;
      }
    }
  }

  return minDistance;
}

// Chapter 3 - Problem 46
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

// Chapter 3 - Problem 47
struct ParsedBooleanExpression {
  std::vector<std::string> postfixExpressionList;
  std::set<std::string> variables;
};

auto splitBooleanExpressionIntoTokens(const std::string &expression);
bool isBooleanOperator(const std::string &token);
bool isBooleanVariable(const std::string &token);
std::optional<bool>
booleanOperatorHasHigherPrecedence(const std::string &operator1,
                                   const std::string &operator2);
std::optional<ParsedBooleanExpression>
parseBooleanExpression(const std::string &expression);
std::optional<bool> solveBooleanExpression(
    const std::vector<std::string> &postfixExpressionList,
    const std::unordered_map<std::string, bool> &variablesValue);

bool isSameBooleanFunction(const std::string &expressionA,
                           const std::string &expressionB);
} // namespace SCO
