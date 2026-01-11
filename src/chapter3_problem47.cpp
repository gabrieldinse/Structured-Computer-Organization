#include "chapter3_problem47.hpp"

#include <cctype>
#include <optional>
#include <print>
#include <regex>
#include <unordered_map>
#include <unordered_set>

namespace SCO {
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
