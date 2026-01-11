#include "Problems.hpp"

#include <cctype>
#include <iostream>
#include <optional>
#include <print>
#include <regex>
#include <unordered_map>
#include <unordered_set>

namespace SCO {
// Chapter 2 - Problem 35
std::int16_t hamming(char ascii) {
  // p1 p2 d1 p3 d2 d3 d4 p4 d5 d6 d7
  char p1 = getBit(ascii, 0) ^ getBit(ascii, 1) ^ getBit(ascii, 3) ^
            getBit(ascii, 4) ^ getBit(ascii, 6);
  char p2 = getBit(ascii, 0) ^ getBit(ascii, 2) ^ getBit(ascii, 3) ^
            getBit(ascii, 5) ^ getBit(ascii, 6);
  char p3 = getBit(ascii, 1) ^ getBit(ascii, 2) ^ getBit(ascii, 3);
  char p4 = getBit(ascii, 4) ^ getBit(ascii, 5) ^ getBit(ascii, 6);

  std::int16_t result = (p1 << 0) | (p2 << 1) | (getBit(ascii, 0) << 2) |
                        (p3 << 3) | (getBit(ascii, 1) << 4) |
                        (getBit(ascii, 2) << 5) | (getBit(ascii, 3) << 6) |
                        (p4 << 7) | (getBit(ascii, 4) << 8) |
                        (getBit(ascii, 5) << 9) | (getBit(ascii, 6) << 10);
  return result;
}

// Chapter 3 - Problem 46
std::ostream &operator<<(std::ostream &os,
                         const std::vector<NANDGateArrayResult> &results) {
  if (results.empty())
    return os;

  // Print Header
  os << "Inputs ";
  for (size_t i = 0; i < results[0].inputs.size(); ++i)
    os << " ";
  os << "| Outputs" << std::endl;
  os << std::string(results[0].inputs.size() + results[0].outputs.size() + 10,
                    '-')
     << std::endl;

  for (const auto &res : results) {
    // Print Inputs as 0s and 1s
    for (bool in : res.inputs) {
      os << (in ? "1 " : "0 ");
    }

    os << "| ";

    // Print Outputs as 0s and 1s
    for (bool out : res.outputs) {
      os << (out ? "1 " : "0 ");
    }
    os << std::endl;
  }

  return os;
}

std::vector<NANDGateArrayResult>
simulateGateArray(std::size_t numRows, std::size_t numCols,
                  std::size_t numInputs, std::size_t numOutputs,
                  std::vector<WireConnection> wireConnections) {
  const std::size_t valuesSize = numInputs + numRows * numCols + numOutputs + 1;
  const std::size_t unusedInputIndex = valuesSize - 1;
  std::vector<NANGGateInputMapping> nandGatesInputMapping(
      numRows * numCols,
      NANGGateInputMapping{unusedInputIndex, unusedInputIndex});
  std::vector<std::size_t> outputPins(numOutputs, unusedInputIndex);

  // Create mapping
  for (const auto &[signalSource, signalDestination] : wireConnections) {
    std::size_t inputIndex = unusedInputIndex;
    if (signalSource.type == SignalSourceType::InputPin) {
      inputIndex = std::get<InputPin>(signalSource.source).index;
    } else if (signalSource.type == SignalSourceType::GateOutput) {
      auto gatePos = std::get<GateOutput>(signalSource.source).gatePos;
      inputIndex = numInputs + (gatePos.row * numCols + gatePos.col);
    }

    if (signalDestination.type == SignalDestinationType::OutputPin) {
      std::size_t outputPinIndex =
          std::get<OutputPin>(signalDestination.destination).index;
      outputPins[outputPinIndex] = inputIndex;
    } else if (signalDestination.type == SignalDestinationType::GateInput) {
      auto gatePos = std::get<GateInput>(signalDestination.destination).gatePos;
      GateInputName gateInputName =
          std::get<GateInput>(signalDestination.destination).gateInputName;
      std::size_t gateInputIndex =
          gateInputName == GateInputName::InputA ? 0 : 1;
      nandGatesInputMapping[gatePos.row * numCols + gatePos.col]
          .inputs[gateInputIndex] = inputIndex;
    }
  }

  // Simulation
  const std::size_t totalCombinations = 1 << numInputs;
  std::vector<NANDGateArrayResult> result(totalCombinations);
  for (std::size_t i = 0; i < totalCombinations; i++) {
    std::vector<bool> oldValues(numInputs + numRows * numCols + numOutputs + 1,
                                true);
    std::vector<bool> values(numInputs + numRows * numCols + numOutputs + 1,
                             true);
    bool valuesChanged = true;
    for (std::size_t bit = 0; bit < numInputs; bit++) {
      oldValues[bit] = values[bit] = (i >> bit) & 1;
      result[i].inputs.push_back(values[bit]);
    }

    // Stabilization loop
    std::print("Inputs #{}\n", i);
    for (std::size_t stabilizationRetry = 0;
         valuesChanged and
         (stabilizationRetry < nandGatesInputMapping.size() + 1);
         stabilizationRetry++) {
      std::print("Retry #{}\n", stabilizationRetry);
      for (std::size_t gatePos = 0; gatePos < nandGatesInputMapping.size();
           gatePos++) {
        auto inputAIndex = nandGatesInputMapping[gatePos].inputs[0];
        auto inputBIndex = nandGatesInputMapping[gatePos].inputs[1];
        bool inputA = oldValues[inputAIndex];
        bool inputB = oldValues[inputBIndex];
        bool output = nand(inputA, inputB);
        values[numInputs + gatePos] = output;
      }

      valuesChanged = values != oldValues;
      oldValues.swap(values);
    }

    for (std::size_t j = 0; j < outputPins.size(); j++) {
      std::size_t outputIndex = outputPins[j];
      result[i].outputs.push_back(values[outputIndex]);
    }
  }

  return result;
}

// Chapter 3 - Problem 47
auto splitBooleanExpressionIntoTokens(const std::string &expression) {
  static const std::regex pattern(R"(\w+|[()])");
  auto matches = std::ranges::subrange(
      std::sregex_iterator(expression.begin(), expression.end(), pattern),
      std::sregex_iterator());

  return matches;
}

bool isBooleanOperator(const std::string &token) {
  static const std::unordered_set<std::string> operators = {"AND", "OR", "NOT"};

  return operators.contains(token);
}

bool isBooleanVariable(const std::string &token) {
  return token.size() == 1 and isalpha(token[0]);
}

std::optional<bool>
booleanOperatorHasHigherPrecedence(const std::string &operator1,
                                   const std::string &operator2) {
  static const std::unordered_map<std::string, int> operatorsValue = {
      {"OR", 1}, {"AND", 2}, {"NOT", 3}};

  auto operator1Val = operatorsValue.find(operator1);
  if (operator1Val == operatorsValue.end()) {
    std::print("Error: unknown operator 1 '{}'.", operator1);
    return std::nullopt;
  }

  auto operator2Val = operatorsValue.find(operator2);
  if (operator2Val == operatorsValue.end()) {
    std::print("Error: unknown operator 2 '{}'.", operator2);
    return std::nullopt;
  }

  return operator1Val->second >= operator2Val->second;
}

std::optional<ParsedBooleanExpression>
parseBooleanExpression(const std::string &expression) {
  ParsedBooleanExpression result;
  std::stack<std::string> operatorsStack;

  std::print("Parsing Expression: '{}'\n", expression);
  for (const auto &match : splitBooleanExpressionIntoTokens(expression)) {
    const std::string &token = match.str();
    std::print("Original Token: '{}'\n", token);
    if (isBooleanVariable(token)) {
      result.variables.insert(token);
      result.postfixExpressionList.push_back(token);
    } else if (isBooleanOperator(token)) {
      while (not operatorsStack.empty() and operatorsStack.top() != "(") {
        auto hasHigherPrecedenceOpt =
            booleanOperatorHasHigherPrecedence(operatorsStack.top(), token);
        if (not hasHigherPrecedenceOpt) {
          std::print("Error: failed to compare operator precedence.\n");
          return std::nullopt;
        }

        if (not *hasHigherPrecedenceOpt) {
          break;
        }

        result.postfixExpressionList.push_back(operatorsStack.top());
        operatorsStack.pop();
      }
      operatorsStack.push(token);
    } else if (token == "(") {
      operatorsStack.push(token);
    } else if (token == ")") {
      while (not operatorsStack.empty() and operatorsStack.top() != "(") {
        result.postfixExpressionList.push_back(operatorsStack.top());
        operatorsStack.pop();
      }

      if (operatorsStack.empty()) {
        std::print("Error: unmatched ')");
        return std::nullopt;
      }

      operatorsStack.pop(); // Pop the '('
    } else {
      std::print("Error: unknown token '{}'.", token);
      return std::nullopt;
    }
  }

  while (not operatorsStack.empty()) {
    if (operatorsStack.top() == "(") {
      std::print("Error: unmatched '('.");
      return std::nullopt;
    }

    result.postfixExpressionList.push_back(operatorsStack.top());
    operatorsStack.pop();
  }

  std::print("Variables:\n");
  for (const auto &var : result.variables) {
    std::print("  {}\n", var);
  }

  std::print("Expression Stack:\n");
  for (const auto &token : result.postfixExpressionList) {
    std::print("  {}\n", token);
  }

  return result;
}

std::optional<bool> solveBooleanExpression(
    const std::vector<std::string> &postfixExpressionList,
    const std::unordered_map<std::string, bool> &variablesValue) {
  std::stack<bool> solvingStack;

  for (const auto &token : postfixExpressionList) {
    if (isBooleanVariable(token)) {
      if (auto it = variablesValue.find(token); it != variablesValue.end()) {
        bool variableValue = it->second;
        solvingStack.push(variableValue);
      } else {
        return std::nullopt;
      }
    } else {
      if (token == "NOT") {
        bool value = solvingStack.top();
        solvingStack.pop();
        solvingStack.push(not value);
      } else {
        bool valueA = solvingStack.top();
        solvingStack.pop();
        bool valueB = solvingStack.top();
        solvingStack.pop();

        if (token == "AND") {
          solvingStack.push(valueA and valueB);
        } else if (token == "OR") {
          solvingStack.push(valueA or valueB);
        } else {
          std::print("Error: unknown operator '{}'.", token);
          return std::nullopt;
        }
      }
    }
  }

  if (solvingStack.size() != 1) {
    return std::nullopt;
  }

  return solvingStack.top();
}

std::optional<std::vector<bool>> simulateBooleanExpression(
    const ParsedBooleanExpression &parsedBooleanExpression) {
  std::vector<bool> result;
  std::size_t numVariables = parsedBooleanExpression.variables.size();
  std::vector<std::string> variablesList(
      parsedBooleanExpression.variables.begin(),
      parsedBooleanExpression.variables.end());
  const std::size_t totalCombinations = 1 << numVariables;
  std::unordered_map<std::string, bool> variablesValue;

  for (std::size_t i = 0; i < totalCombinations; i++) {
    for (std::size_t bit = 0; bit < numVariables; bit++) {
      bool value = (i >> bit) & 1;
      variablesValue[variablesList[bit]] = value;
    }
    std::print("Simulating combination: ");
    for (const auto &[var, val] : variablesValue) {
      std::print("{}={}, ", var, val ? "1" : "0");
    }
    std::print("\n");

    auto resultOpt = solveBooleanExpression(
        parsedBooleanExpression.postfixExpressionList, variablesValue);
    if (resultOpt) {
      result.push_back(*resultOpt);
    } else {
      return std::nullopt;
    }
  }

  return result;
}

bool isSameBooleanFunction(const std::string &expressionA,
                           const std::string &expressionB) {
  auto parsedBooleanExpressionAOpt = parseBooleanExpression(expressionA);
  if (not parsedBooleanExpressionAOpt) {
    std::print("Failed to parse expression A '{}'\n", expressionA);
    return false;
  }

  auto parsedBooleanExpressionBOpt = parseBooleanExpression(expressionB);
  if (not parsedBooleanExpressionBOpt) {
    std::print("Failed to parse expression B '{}'\n", expressionB);
    return false;
  }

  const ParsedBooleanExpression &parsedBooleanExpressionA =
      *parsedBooleanExpressionAOpt;
  const ParsedBooleanExpression &parsedBooleanExpressionB =
      *parsedBooleanExpressionBOpt;
  if (parsedBooleanExpressionA.variables !=
      parsedBooleanExpressionB.variables) {
    std::print("Expressions have different variables.\n");
    return false;
  }

  auto expressionAResultsOpt =
      simulateBooleanExpression(parsedBooleanExpressionA);
  if (not expressionAResultsOpt) {
    std::print("Failed to simulate expression A '{}'\n", expressionA);
    return false;
  }

  auto expressionBResultsOpt =
      simulateBooleanExpression(parsedBooleanExpressionB);
  if (not expressionBResultsOpt) {
    std::print("Failed to simulate expression B '{}'\n", expressionB);
    return false;
  }

  return *expressionAResultsOpt == *expressionBResultsOpt;
}
} // namespace SCO
