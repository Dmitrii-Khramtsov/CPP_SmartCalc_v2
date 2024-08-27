// Copyright 2024 Dmitrii Khramtsov

/**
 * @file calc_controller.cc
 *
 * @brief Implementation of the CalcController class methods for the SmartCalc
 * v2.0 library.
 *
 * This file contains the implementation of the methods for the CalcController
 * class, which is part of the SmartCalc v2.0 library. The CalcController class
 * acts as an intermediary between the user interface and the model, handling
 * the calculation of mathematical expressions.
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-08-12
 *
 * @copyright School-21 (c) 2024
 */

#include "calc_controller.h"

#include <iostream>

/**
 * @brief Calculate the result of a mathematical expression.
 *
 * @param expression The mathematical expression to calculate.
 * @return The result of the calculation.
 */
double s21::CalcController::calculateExpression(const String& expression, const double& x) {
  return model_.calculate(expression, x);
}

/**
 * @brief Calculate the credit payments based on the specified type.
 *
 * @param type The type of monthly payment calculation (ANNUITY or
 * DIFFERENTIAL).
 * @param in The input data for the calculation.
 * @param monthly_pay The calculated monthly payment (used for annuity
 * calculation).
 * @param out The Croutput data containing the total and overpayment.
 * @param payments The vector of payments (used for differential calculation).
 */
void s21::CalcController::calculateCredit(TypeOfMonthlyPayments type,
                                          CrInput in, double& monthly_pay,
                                          CrOutput& out,
                                          PaymentVector& payments) {
  credit_.calculateCredit(type, in, monthly_pay, out, payments);
}

std::vector<std::vector<double>> s21::CalcController::calculateGraf(
    std::pair<double, double> xRange, std::pair<double, double> yRange,
    unsigned pAmount, std::string infix){
    return model_.calculateGraf(xRange, yRange, pAmount, infix);
}

/**
 * @brief Calculate the deposit based on the input parameters.
 *
 * @param in The input parameters for the deposit calculation.
 * @param out The output parameters to be updated with the calculation results.
 */
void s21::CalcController::calculateDeposit(const Input& in, Output& out) {
  deposit_.calculateDeposit(in, out);
}
