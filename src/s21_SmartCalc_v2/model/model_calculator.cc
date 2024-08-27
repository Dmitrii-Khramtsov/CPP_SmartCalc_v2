// Copyright 2024 Dmitrii Khramtsov

/**
 * @file model_calculator.cc
 *
 * @brief Implementation of the ModelCalculator class
 * for the SmartCalc v2.0 library.
 *
 * This file contains the implementation of the ModelCalculator class,
 * which is part of the SmartCalc v2.0 library.
 * The ModelCalculator class is responsible for evaluating mathematical
 * expressions using Reverse Polish Notation (RPN).
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-08-11
 *
 * @copyright School-21 (c) 2024
 */

#include "model_calculator.h"

#include <iostream>

#include "polish_notation.h"

namespace s21 {

/******************************************************************************
 * MAIN METHODS
 ******************************************************************************/

/**
 * @brief Calculates the result of a mathematical expression.
 *
 * @param expression The mathematical expression to be evaluated.
 * @param x The value to substitute for 'x' in the expression.
 * @return The result of the evaluated expression.
 * @throw std::invalid_argument If the expression is invalid.
 */
double ModelCalculator::calculate(const String &expression, const double &x) {
  String rpn = ReversePolishNotation::toRPN(expression);
  answer_ = evaluateRPN(rpn, x);
  return answer_;
}

Vector ModelCalculator::calculateGraf(std::pair<double, double> xRange,
                                      std::pair<double, double> yRange,
                                      unsigned pAmount, std::string infix) {
  std::vector<std::vector<double>> vXYOutPut(2, std::vector<double>());
  double vXStep = (xRange.second - xRange.first) / pAmount;
  double vX = xRange.first;
  double vY = 0;
  if ((xRange.second < xRange.first) || (yRange.second < yRange.first)) {
    throw std::invalid_argument(
        "Не коректно введены граници отображения графика");
  }
  String rpn = ReversePolishNotation::toRPN(infix);
  for (unsigned i = 0; i < pAmount; i++) {
    try {
      vY = evaluateRPN(rpn, vX);
      if (vY >= yRange.first && vY <= yRange.second) {
        vXYOutPut[1].push_back(vY);
        vXYOutPut[0].push_back(vX);
      }
    } catch (std::exception const &) {
      vXYOutPut[1].push_back(NAN);
      vXYOutPut[0].push_back(NAN);
    }

    vX += vXStep;
  }
  if (vXYOutPut[0].empty()) {
    throw std::invalid_argument(
        "ни одна из точек не находится в заданной области значений");
  }
  return vXYOutPut;
}

/******************************************************************************
 * AUXILIARY PRIVATE MAIN METHODS
 ******************************************************************************/

/**
 * @brief Evaluates an expression in Reverse Polish Notation (RPN).
 *
 * @param expression The expression in RPN to be evaluated.
 * @param x The value to substitute for 'x' in the expression.
 * @return The result of the evaluated expression.
 * @throw std::invalid_argument If the expression is invalid.
 */
double ModelCalculator::evaluateRPN(const String &rpn, const double &x) {
  std::stack<double> stack;
  String token;
  std::istringstream iss(rpn);

  while (iss >> token) {
    double a, b;
    switch (tokenType(token)) {
      case TYPE_OPERAND:
        stack.push(token == "x" ? x : std::stod(token));
        break;
      case TYPE_BINARY_OPERATOR:
        if (stack.size() < 2) {
          throw std::invalid_argument(
              "Invalid RPN expression: not enough operands");
        }
        b = stack.top();
        stack.pop();
        a = stack.top();
        stack.pop();
        stack.push(applyBinaryOperator(token[0], a, b));
        break;
      case TYPE_UNARY_OPERATOR:
        if (stack.empty()) {
          throw std::invalid_argument(
              "Invalid RPN expression: not enough operands");
        }
        a = stack.top();
        stack.pop();
        stack.push(applyUnaryOperator(token[0], a));
        break;
      default:
        throw std::invalid_argument("Unknown token: " + token);
    }
  }

  if (stack.size() != 1) {
    throw std::invalid_argument("Invalid RPN expression");
  }

  return stack.top();
}

/**
 * @brief Determines the type of a token.
 *
 * @param token The token to be classified.
 * @param x The value to substitute for 'x' in the expression.
 * @return The type of the token.
 */
TokenType ModelCalculator::tokenType(const String &token) {
  if (isOperand(token)) {
    return TYPE_OPERAND;
  } else if (isBinaryOperator(token[0])) {
    return TYPE_BINARY_OPERATOR;
  } else if (isUnaryOperator(token[0])) {
    return TYPE_UNARY_OPERATOR;
  } else {
    return TYPE_UNKNOWN;
  }
}

/**
 * @brief Checks if a token is an operand.
 *
 * @param token The token to be checked.
 * @param x The value to substitute for 'x' in the expression.
 * @return True if the token is an operand, false otherwise.
 */
bool ModelCalculator::isOperand(const String &token) {
  if (token == "x") {
    return true;
  }
  std::istringstream iss(token);
  double d;
  return (iss >> d).fail() == false;
}

/**
 * @brief Checks if a character is a binary operator.
 *
 * @param b_op The character to be checked.
 * @return True if the character is a binary operator, false otherwise.
 */
bool ModelCalculator::isBinaryOperator(const char b_op) {
  static const std::set<char> binary_operators = {'+', '-', '*', '/', '^', '%'};
  return binary_operators.find(b_op) != binary_operators.end();
}

/**
 * @brief Checks if a character is a unary operator.
 *
 * @param u_op The character to be checked.
 * @return True if the character is a unary operator, false otherwise.
 */
bool ModelCalculator::isUnaryOperator(const char u_op) {
  static const std::set<char> unary_operators = {'s', 'c', 't', 'i', 'o',
                                                 'n', 'q', 'l', 'g', '~'};
  return unary_operators.find(u_op) != unary_operators.end();
}

/**
 * @brief Applies a binary operator to two operands.
 *
 * @param b_op The binary operator.
 * @param a The first operand.
 * @param b The second operand.
 * @return The result of the operation.
 * @throw std::invalid_argument If the operator is unknown
 * or division by zero occurs.
 */
double ModelCalculator::applyBinaryOperator(const char b_op, double a,
                                            double b) {
  switch (b_op) {
    case '+':
      return a + b;
    case '-':
      return a - b;
    case '*':
      return a * b;
    case '/':
      if (b == 0.0) {
        throw std::invalid_argument("Division by zero");
      }
      return a / b;
    case '^':
      return std::pow(a, b);
    case '%':
      if (b == 0.0) {
        throw std::invalid_argument("Division by zero");
      }
      return std::fmod(a, b);
    default:
      throw std::invalid_argument("Unknown operator");
  }
}

/**
 * @brief Applies a unary operator to an operand.
 *
 * @param u_op The unary operator.
 * @param a The operand to which the operator is applied.
 * @return The result of the operation.
 * @throw std::invalid_argument If the operator is unknown
 * or the operand is invalid for the operator.
 */
double ModelCalculator::applyUnaryOperator(const char u_op, double a) {
  switch (u_op) {
    case 's':
      return std::sin(a);
    case 'c':
      return std::cos(a);
    case 't':
      return std::tan(a);
    case 'i':
      return std::asin(a);
    case 'o':
      return std::acos(a);
    case 'n':
      return std::atan(a);
    case 'q':
      if (a < 0.0) {
        throw std::invalid_argument(
            "The expression under the root cannot be negative");
      }
      return std::sqrt(a);
    case 'l':
      if (a <= 0.0) {
        throw std::invalid_argument(
            "The expression under the logarithm cannot be zero or negative.");
      }
      return std::log(a);
    case 'g':
      if (a <= 0.0) {
        throw std::invalid_argument(
            "The expression under the logarithm cannot be zero or negative.");
      }
      return std::log10(a);
    case '~':
      return -a;
    default:
      throw std::invalid_argument("Unknown unary operator");
  }
}

}  // namespace s21
