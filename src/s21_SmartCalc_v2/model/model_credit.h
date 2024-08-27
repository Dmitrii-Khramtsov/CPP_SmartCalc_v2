// Copyright 2024 Dmitrii Khramtsov

/**
 * @file model_credit.h
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @brief Implementation of the CreditModel class
 * for calculating annuity and differential payments.
 *
 * @date 2024-08-13
 *
 * @copyright School-21 (c) 2024
 */

#ifndef CPP3_S21_SMART_CALC_MODEL_CREDIT_H
#define CPP3_S21_SMART_CALC_MODEL_CREDIT_H

#include <iostream>
#include <cmath>
#include <ctime>    // getCurrentDate, getCurrentDateAndTime
#include <iomanip>  // getCurrentDate, getCurrentDateAndTime
#include <sstream>  // getCurrentDate, formatDate
#include <string>   // calculateDifferential, getCurrentDate, formatDate
#include <vector>   // calculateDifferential

namespace s21 {

enum TypeOfMonthlyPayments { ANNUITY, DIFFERENTIAL };

struct CrInput {
  double credit;
  int term;
  double rate;
};

struct CrOutput {
  double total;
  double overpayment;
};

struct Payment {
  double monthly_pay; // ежемесячный платёж
  double interest_pay; // платёж по процентам
  std::string date; // число платежа
  double total_reminder; // остаток долга
  double const_payment; // платёж по основному долгу
};

using PaymentVector = std::vector<Payment>;
using String = std::string;

class CreditModel {
 public:
  void calculateCredit(TypeOfMonthlyPayments type, CrInput in,
                       double& monthly_pay, CrOutput& out,
                       PaymentVector& payments);

 private:
  void calculateAnnuity(CrInput in, double& monthly_pay, CrOutput& out);
  PaymentVector calculateDifferential(CrInput in, double& first_month_pay, CrOutput& out);
  void calculateMonthlyPayment(double& total_reminder, double const_payment, double const_rate, double& payment, double& interest, CrOutput& out);
  double calculateFirstMonthPayment(double total_reminder, double const_rate, double const_payment);
  String getCurrentDate();
  bool isLeapYear(int year);
  int getDaysInYear(int year);
  int getDaysInMonth(int month, int year);
  void getCurrentDateAndTime(int& current_day, int& current_month,
                             int& current_year);
  String formatDate(int day, int month, int year);
  void incrementMonthAndYear(int& current_day, int& current_month,
                             int& current_year);

  enum Month {
    UNKNOWN,
    JANUARY,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
  };
};

}  // namespace s21

#endif  // CPP3_S21_SMART_CALC_MODEL_CREDIT_H
