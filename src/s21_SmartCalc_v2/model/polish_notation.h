#ifndef CPP3_S21_SMARTCALC_REVERSE_POLISH_NOTATION_H
#define CPP3_S21_SMARTCALC_REVERSE_POLISH_NOTATION_H

#include <iostream>
#include <string>        // toRPN, renameFunctions, replaceUnaryMinus, processedInfix
#include <stack>         // toRPN
#include <unordered_set> // isOperator, isFunction
#include <unordered_map> // operatorPriority, functions
#include <cctype>        // isOperand, processExponent (std::isdigit)
#include <stdexcept>     // getOperatorPriority

namespace s21 {

enum OperatorPriority {
  OP_PARENTHESIS = 0,    // Приоритет скобок '(', ')' (не влияет на операции)
  OP_ADDITIVE = 1,       // Приоритет сложения и вычитания (+, -)
  OP_MULTIPLICATIVE = 2, // Приоритет умножения, деления и остатка (*, /, %)
  OP_FUNCTION = 3,       // Приоритет функций ('s', 'c'..)
  OP_EXPONENTIAL = 4,    // Приоритет возведения в степень (^)
  OP_UNARY_MINUS = 5     // Приоритет унарного минуса (~)
};

enum isOperandOrOperatorOrFunction {
  BRACKETS = 0,
  OPERAND = 1,
  OPERATOR = 2,
  FUNCTION = 3
};

using String = std::string;
using StringStack = std::stack<std::string>;
using FunctionMap = std::unordered_map<std::string, char>;
using OperatorPriorityMap = std::unordered_map<char, OperatorPriority>;

class ReversePolishNotation {
 public:
  // Main methods:
  static String toRPN(const String& infix);

 private:
  // Auxiliary methods:
  static const OperatorPriorityMap operatorPriority;
  static const FunctionMap functions;

  static int getOperatorPriority(const char c);

  static bool isOperand(const char c);
  static bool isOperator(const char op);
  static bool isFunction(const char fn);
  static int isOperandOrOperatorOrFunction(const String& infix, size_t i);

  static void processOperand(String& output, const String& token);

  static void popAndAppendOperator(StringStack& operators, String& output);
  static void flushOperators(StringStack& operators, String& output);

  static String renameFunctions(const String& infix);
  static String replaceUnaryMinus(const String& infix);
  static String processedInfix(const String& infix);

  static void flushToken(String& output, String& token);

  static bool isPartOfExponent(const char c);
  static void processExponent(const String& infix, size_t& i, String& token);
  static void findScientificNumber(const String &infix, size_t &index, String &postfix);

  static void handleOperand(String& output, String& token, const String& infix, size_t& i);
  static bool isUnaryMinus(const String& infix, size_t i);
  static void handleUnaryMinus(String& output, String& token, StringStack& operators);
  static void handleOperator(String& output, String& token, StringStack& operators, char c);
  static void handleFunction(String& output, String& token, StringStack& operators, char c);
  static void handleParenthesis(String& output, String& token, StringStack& operators, char c);

  static String trim(const String& str);
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_REVERSE_POLISH_NOTATION_H
