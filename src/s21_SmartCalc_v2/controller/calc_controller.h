// Copyright 2024 Dmitrii Khramtsov

/**
 * @file calc_controller.h
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

#ifndef CPP3_S21_SMART_CALC_CALC_CONTROLLER_H
#define CPP3_S21_SMART_CALC_CALC_CONTROLLER_H

#include "../model/model_calculator.h"
#include "../model/model_credit.h"
#include "../model/model_deposit.h"

namespace s21 {
class CalcController {
 public:
  CalcController() = default;
  double calculateExpression(const String& expression, const double& x);
  void calculateCredit(TypeOfMonthlyPayments type, CrInput in,
                       double& monthly_pay, CrOutput& out,
                       PaymentVector& payments);
  void calculateDeposit(const Input& in, Output& out);
  Vector calculateGraf(
      std::pair<double, double> xRange, std::pair<double, double> yRange,
      unsigned pAmount, std::string infix);

 private:
  ModelCalculator model_;
  CreditModel credit_;
  DepositModel deposit_;
};

}  // namespace s21

#endif  // CPP3_S21_SMART_CALC_CALC_CONTROLLER_H
