// Copyright 2024 Dmitrii Khramtsov

/**
 * @file model_credit.cc
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

#include "model_credit.h"

namespace s21 {

/******************************************************************************
 * MAIN METHODS
 ******************************************************************************/

/**
 * @brief Calculate the monthly payments based on the specified type.
 *
 * @param type The type of monthly payment calculation
 * (ANNUITY or DIFFERENTIAL).
 * @param in The input data for the calculation.
 * @param monthly_pay The calculated monthly payment
 * (used for annuity calculation).
 * @param out The output data containing the total and overpayment.
 * @param payments The vector of payments (used for differential calculation).
 */
void CreditModel::calculateCredit(TypeOfMonthlyPayments type, CrInput in,
                                  double &monthly_pay, CrOutput &out,
                                  PaymentVector &payments) {
  if (type == ANNUITY) {
    calculateAnnuity(in, monthly_pay, out);
  } else if (type == DIFFERENTIAL) {
    payments = calculateDifferential(in, monthly_pay, out);
  }
}

/******************************************************************************
 * AUXILIARY PRIVATE MAIN METHODS
 ******************************************************************************/

/**
 * @brief Calculate the annuity payment.
 *
 * @param in The input data for the calculation.
 * @param monthly_pay The calculated monthly payment.
 * @param out The output data containing the total and overpayment.
 */
void CreditModel::calculateAnnuity(CrInput in, double &monthly_pay,
                                   CrOutput &out) {
  monthly_pay = round(in.credit * (in.rate / 1200) /
                      (1 - pow(1 + in.rate / 1200, -in.term)) * 100) /
                100;
  out.total = monthly_pay * in.term;
  out.overpayment = out.total - in.credit;
}

/**
 * @brief Calculate the differential payments.
 *
 * This function calculates the differential payments for a given credit term
 * and rate. It returns a vector of payments, each containing the payment
 * amount, the remaining debt, and the formatted date of the payment.
 *
 * @param in The input data for the calculation, including the credit amount,
 * term and rate.
 * @param first_month_pay The calculated payment for the first month.
 * @param out The output data containing the total payment and overpayment.
 * @return A vector of payments, each containing the payment amount,
 * the remaining debt and the formatted date.
 */
PaymentVector CreditModel::calculateDifferential(CrInput in, double& first_month_pay, CrOutput &out) {
    PaymentVector payments;
    double total_reminder = in.credit;
    double const_payment = in.credit / in.term;
    double const_rate = in.rate / 12 / 100; // процентная ставка в месяц
    double monthly_pay;
    double interest_pay;
    out.total = 0;

    int current_day, current_month, current_year;
    getCurrentDateAndTime(current_day, current_month, current_year);

    // вычисляем платёж в первый месяц
    first_month_pay = calculateFirstMonthPayment(total_reminder, const_rate, const_payment);

    for (int i = 0; i < in.term; i++) {
        // вычисляем платеж за месяц
        calculateMonthlyPayment(total_reminder, const_payment, const_rate, monthly_pay, interest_pay, out);

        // отформатируем дату и добавим детали платежа в вектор платежей
        std::string formatted_date = formatDate(current_day, current_month, current_year);
        payments.push_back({monthly_pay, interest_pay, formatted_date, total_reminder, const_payment});

        // увеличение месяца и года
        incrementMonthAndYear(current_day, current_month, current_year);
    }

    // расчитаем переплату
    out.overpayment = out.total - in.credit;
    return payments;
}

/**
 * @brief Calculate the monthly payment based on the credit, rate,
 * days in year and days in month.
 *
 * This function calculates the monthly payment for a given credit amount, rate,
 * number of days in the month, and number of days in the year.
 *
 * @param total_reminder The remaining credit amount.
 * @param const_payment The constant payment part.
 * @param const_rate The interest rate.
 * @param monthly_pay The calculated monthly payment.
 * @param interest_pay The calculated interest for the month.
 * @param out The output data containing the total payment and overpayment.
 */
void CreditModel::calculateMonthlyPayment(double& total_reminder, double const_payment, double const_rate, double& monthly_pay, double& interest_pay, CrOutput& out) {
    // Вычисляем проценты на оставшуюся сумму долга
    interest_pay = total_reminder * const_rate;

    // общий платеж за месяц
    monthly_pay = const_payment + interest_pay;

    // обновляем оставшуюся сумму долга
    total_reminder -= const_payment;

    // добавляем общий платеж к общей сумме
    out.total += monthly_pay;
}

/**
 * @brief Calculate the first month payment.
 *
 * This function calculates the first month payment for a given credit amount, rate,
 * current month, current year, and constant payment.
 *
 * @param total_reminder The remaining credit amount.
 * @param const_rate The interest rate.
 * @param const_payment The constant payment part.
 * @return The calculated first month payment.
 */
double CreditModel::calculateFirstMonthPayment(double total_reminder, double const_rate, double const_payment) {
    return const_payment + total_reminder * const_rate;
}


/**
 * @brief Get the current date as a string.
 *
 * @return The current date as a string in the format "YYYY-MM-DD".
 */
String CreditModel::getCurrentDate() {
  std::time_t now_time = std::time(nullptr);
  std::tm *now_tm = std::localtime(&now_time);
  std::ostringstream oss;
  oss << std::put_time(now_tm, "%Y-%m-%d");
  return oss.str();
}

/**
 * @brief Check if a year is a leap year.
 *
 * @param year The year to check.
 * @return True if the year is a leap year, false otherwise.
 */
bool CreditModel::isLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * @brief Get the number of days in a year.
 *
 * @param year The year to check.
 * @return The number of days in the year (366 for leap years, 365 otherwise).
 */
int CreditModel::getDaysInYear(int year) {
  return isLeapYear(year) ? 366 : 365;
}

/**
 * @brief Get the number of days in a month.
 *
 * @param month The month to check.
 * @param year The year to check.
 * @return The number of days in the month.
 */
int CreditModel::getDaysInMonth(int month, int year) {
  switch (month) {
    case JANUARY:
    case MARCH:
    case MAY:
    case JULY:
    case AUGUST:
    case OCTOBER:
    case DECEMBER:
      return 31;
    case APRIL:
    case JUNE:
    case SEPTEMBER:
    case NOVEMBER:
      return 30;
    case FEBRUARY:
      return isLeapYear(year) ? 29 : 28;
    default:
      return UNKNOWN;
  }
}

/**
 * @brief Get the current date and time.
 *
 * @param current_day The current day.
 * @param current_month The current month.
 * @param current_year The current year.
 */
void CreditModel::getCurrentDateAndTime(int &current_day, int &current_month,
                                        int &current_year) {
  std::time_t now_time = std::time(nullptr);
  std::tm *now_tm = std::localtime(&now_time);
  current_year = now_tm->tm_year + 1900;
  current_month = now_tm->tm_mon + 1;
  current_day = now_tm->tm_mday;
}

/**
 * @brief Format a date as a string in the format "DD.MM.YYYY".
 *
 * @param day The day of the month.
 * @param month The month of the year.
 * @param year The year.
 * @return The formatted date as a string.
 */
String CreditModel::formatDate(int day, int month, int year) {
  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << day << "." << std::setw(2)
      << std::setfill('0') << month << "." << std::setw(4) << std::setfill('0')
      << year;
  return oss.str();
}

/**
 * @brief Increment the month and year.
 *
 * @param current_day The current day.
 * @param current_month The current month.
 * @param current_year The current year.
 */
void CreditModel::incrementMonthAndYear(int &current_day, int &current_month,
                                        int &current_year) {
  current_month++;
  if (current_month > 12) {
    current_month = 1;
    current_year++;
  }

  int days_in_month = getDaysInMonth(current_month, current_year);
  if (current_day > days_in_month) {
    current_day = days_in_month;
  }
}

}  // namespace s21
