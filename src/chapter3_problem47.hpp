#pragma once

#include <print>
#include <set>
#include <unordered_map>
#include <vector>

namespace SCO {
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
