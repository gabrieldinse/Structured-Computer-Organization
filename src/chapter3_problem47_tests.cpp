#include <print>
#include <regex>

#include "gtest/gtest.h"

#include "chapter3_problem47.hpp"

namespace SCO {
TEST(RegexTest, Regex) {
  std::regex pattern(R"(\w+|[()])");

  std::string str = "NOT (A AND B) OR C";
  auto tokens = std::ranges::subrange(
      std::sregex_iterator(str.begin(), str.end(), pattern),
      std::sregex_iterator());

  for (const auto &token : tokens) {
    std::print("Match: {}\n", token.str());
  }
}

TEST(IsSameBooleanFunction, ParseBooleanExpression) {
  std::string expression = "NOT (A AND B) OR C";
  auto parsedExpressionOpt = SCO::parseBooleanExpression(expression);
  ASSERT_TRUE(parsedExpressionOpt.has_value());

  auto &parsedExpression = parsedExpressionOpt.value();

  std::string expressionB = "(NOT A OR NOT B) OR C";
  auto parsedExpressionBOpt = SCO::parseBooleanExpression(expressionB);
  ASSERT_TRUE(parsedExpressionBOpt.has_value());
}

TEST(IsSameBooleanFunction, solveBooleanExpression) {
  std::string expression = "NOT (A AND B) OR C";
  auto postfixExpressionOpt = SCO::parseBooleanExpression(expression);
  ASSERT_TRUE(postfixExpressionOpt.has_value());
  std::vector<std::string> postfixExpression =
      postfixExpressionOpt->postfixExpressionList;

  std::unordered_map<std::string, bool> variablesValue = {
      {"A", true}, {"B", false}, {"C", true}};

  auto resultOpt =
      SCO::solveBooleanExpression(postfixExpression, variablesValue);
  ASSERT_TRUE(resultOpt.has_value());
  bool result = resultOpt.value();
  EXPECT_EQ(result, true);

  variablesValue = {{"A", true}, {"B", true}, {"C", false}};
  resultOpt = SCO::solveBooleanExpression(postfixExpression, variablesValue);
  ASSERT_TRUE(resultOpt.has_value());
  result = resultOpt.value();
  EXPECT_EQ(result, false);

  std::string exprB = "(NOT A OR NOT B) OR C";
  auto postfixExpressionBOpt = SCO::parseBooleanExpression(exprB);
  ASSERT_TRUE(postfixExpressionBOpt.has_value());
  std::vector<std::string> postfixExpressionB =
      postfixExpressionBOpt->postfixExpressionList;
  resultOpt = SCO::solveBooleanExpression(postfixExpressionB, variablesValue);
  ASSERT_TRUE(resultOpt.has_value());
  result = resultOpt.value();
  EXPECT_EQ(result, false);
}

TEST(IsSameBooleanFunction, IsSameBooleanFunction) {
  std::string exprA = "NOT (A AND B) OR C";
  std::string exprB = "(NOT A OR NOT B) OR C";

  bool result = SCO::isSameBooleanFunction(exprA, exprB);
  EXPECT_TRUE(result);

  exprB = "A AND (B OR C)";
  result = SCO::isSameBooleanFunction(exprA, exprB);
  EXPECT_FALSE(result);
}
} // namespace SCO