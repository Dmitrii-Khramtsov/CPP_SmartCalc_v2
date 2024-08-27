// Copyright 2024 Dmitrii Khramtsov

/**
 * @file model_calculator.h
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

#ifndef CPP3_S21_SMART_CALC_MODEL_CALCULATOR_H
#define CPP3_S21_SMART_CALC_MODEL_CALCULATOR_H

#include <cmath>  // applyBinaryOperator, applyUnaryOperator
#include <iostream>
#include <set>        // isBinaryOperator, isUnaryOperator
#include <sstream>    // evaluateRPN
#include <stack>      // evaluateRPN
#include <stdexcept>  // evaluateRPN, applyBinaryOperator, applyUnaryOperator
#include <vector>

#include "polish_notation.h"

namespace s21 {

enum TokenType { TYPE_OPERAND, TYPE_BINARY_OPERATOR, TYPE_UNARY_OPERATOR, TYPE_UNKNOWN };

using String = std::string;
using Vector = std::vector<std::vector<double>>;

class ModelCalculator {
   public:
    ModelCalculator() : answer_(0) {};

    // Main methods:
    double calculate(const String& expression, const double& x);
    Vector calculateGraf(std::pair<double, double> xRange, std::pair<double, double> yRange, unsigned pAmount,
                         std::string infix);

   private:
    // Auxiliary methods:
    double evaluateRPN(const String& rpn, const double& x);

    TokenType tokenType(const String& token);

    bool isOperand(const String& token);
    bool isBinaryOperator(const char b_op);
    bool isUnaryOperator(const char u_op);

    double applyUnaryOperator(const char fn, double operand);
    double applyBinaryOperator(const char op, double operand_1, double operand_2);

    double answer_;
};

}  // namespace s21

#endif  // CPP3_S21_SMART_CALC_MODEL_CALCULATOR_H
