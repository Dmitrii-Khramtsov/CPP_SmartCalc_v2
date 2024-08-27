// Copyright 2024 Dmitrii Khramtsov

/**
 * @file model_deposit.cc
 *
 * @brief Implementation of the DepositModel class for the SmartCalc v2.0
 * library.
 *
 * This file contains the implementation of the DepositModel class,
 * which is part of the SmartCalc v2.0 library.
 * The DepositModel class is responsible for calculating deposit-related
 * operations, including interest, taxes, and handling additions and payments.
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-08-17
 *
 * @copyright School-21 (c) 2024
 */

#include "model_deposit.h"

namespace s21 {

DepositModel::DepositModel() {}

/******************************************************************************
 * MAIN METHODS
 ******************************************************************************/

/**
 * @brief Calculates the deposit based on the input parameters
 * and updates the output.
 *
 * @param in Input parameters for the deposit calculation.
 * @param out Output parameters to be updated with the calculation results.
 */
void DepositModel::calculateDeposit(const Input& in, Output& out) {
  // инициализация векторов изменений
  ChangeVector additions = in.additions;
  ChangeVector payments = in.payments;

  // создание объекта данных для расчета
  CalcData data;

  // инициализация данных для расчета
  initializeCalculationData(data, in, additions, payments);

  // инициализация выходных данных
  initializeOutput(out, in);

  // цикл по дням от текущего момента до конца срока вклада
  for (auto current = in.now; current <= data.end;
       current += std::chrono::hours(24)) {
    // обработка текущего дня
    processDay(current, data, out, in, additions, payments);
  }

  // обновление начальной записи отчета
  updateInitialReportRecord(out, in);
}

/******************************************************************************
 * DATA INITIALIZATION
 ******************************************************************************/

/**
 * @brief Initializes the output parameters.
 *
 * @param out Output parameters to be initialized.
 * @param in Input parameters for the deposit calculation.
 */
void DepositModel::initializeOutput(Output& out, const Input& in) {
  // инициализация выходных данных
  out.totalPercents = 0;
  out.totalTax = 0;
  out.remainderDeposit = in.deposit;
}

/**
 * @brief Updates the initial report record with the deposit amount.
 *
 * @param out Output parameters to be updated.
 * @param in Input parameters for the deposit calculation.
 */
void DepositModel::updateInitialReportRecord(Output& out, const Input& in) {
  // обновление начальной записи отчета
  if (!out.report.empty()) {
    out.report.front().addition = in.deposit;
    out.report.front().percents = 0;
  }
}

/**
 * @brief Initializes the calculation data based on the input parameters.
 *
 * @param data Calculation data to be initialized.
 * @param params Input parameters for the deposit calculation.
 * @param additions Vector of addition changes.
 * @param payments Vector of payment changes.
 */
void DepositModel::initializeCalculationData(CalcData& data,
                                             const Input& params,
                                             ChangeVector& additions,
                                             ChangeVector& payments) {
  // инициализация данных для расчета
  auto current = params.now;
  int totalDays = 0;

  // расчет общего количества дней вклада
  for (int i = 0; i < params.monthsTerm; ++i) {
    int daysInMonth = getDaysInMonthFromDate(current);
    totalDays += daysInMonth;
    // переход к следующему месяцу
    current += std::chrono::hours(24 * daysInMonth);
  }

  // установка конечной даты вклада
  data.end = params.now + std::chrono::hours(24 * totalDays);

  // сортировка записей изменений
  sortChangeRecords(additions);
  sortChangeRecords(payments);

  // инициализация вектора дат начисления процентов
  initPercentsVector(data, params);

  // инициализация начальных значений процентов
  data.percents = 0;
  data.percentsByYear = 0;
}

/**
 * @brief Sorts the change records by date.
 *
 * @param changes Vector of change records to be sorted.
 */
void DepositModel::sortChangeRecords(ChangeVector& changes) {
  // сортировка записей изменений по дате
  std::sort(changes.begin(), changes.end(),
            [](const Change& a, const Change& b) { return a.date < b.date; });
}

/**
 * @brief Initializes the vector of percent dates based on the input parameters.
 *
 * @param data Calculation data to be updated.
 * @param params Input parameters for the deposit calculation.
 */
void DepositModel::initPercentsVector(CalcData& data, const Input& params) {
  // инициализация вектора дат начисления процентов
  data.percentsDate.clear();

  auto current = params.now;
  while (current < data.end) {
    switch (params.period) {
      case Period::EveryDay:
        data.percentsDate.push_back(current);
        // переход к следующему дню
        current += std::chrono::hours(24);
        break;
      case Period::EveryMonth:
        data.percentsDate.push_back(current);
        // переход к следующему месяцу
        current += std::chrono::hours(24 * getDaysInMonthFromDate(current));
        break;
      case Period::EveryYear:
        data.percentsDate.push_back(current);
        // переход к следующему году
        current += std::chrono::hours(24 * 365);
        break;
    }
  }

  // добавление конечной даты в вектор
  data.percentsDate.push_back(data.end);
}

/******************************************************************************
 * PROCESSING OF DAYS AND CHANGES
 ******************************************************************************/

/**
 * @brief Processes the current day and updates the calculation data and output.
 *
 * @param current The current date being processed.
 * @param data Calculation data to be updated.
 * @param out Output parameters to be updated.
 * @param params Input parameters for the deposit calculation.
 * @param additions Vector of addition changes.
 * @param payments Vector of payment changes.
 */
void DepositModel::processDay(const TimePoint& current, CalcData& data,
                              Output& out, const Input& params,
                              ChangeVector& additions, ChangeVector& payments) {
  // обработка текущего дня
  data.havingRecord = false;
  Report add;
  add.date = current;
  add.percents = 0;
  add.addition = 0;
  add.remainder = out.remainderDeposit;

  // обработка начисления процентов
  processInterest(current, add, data, out, params);
  // обработка пополнений
  processAdditions(current, add, data, out, additions);
  // обработка снятий
  processPayments(current, add, data, out, payments);

  // добавление записи в отчет
  addToReport(out, add, data.havingRecord);

  // расчет процентов и налогов
  calculateInterestAndTax(current, data, out, params);
}

/**
 * @brief Processes the interest for the current day and updates the calculation
 * data and output.
 *
 * @param current The current date being processed.
 * @param add The report record to be updated.
 * @param data Calculation data to be updated.
 * @param out Output parameters to be updated.
 * @param params Input parameters for the deposit calculation.
 */
void DepositModel::processInterest(const TimePoint& current, Report& add,
                                   CalcData& data, Output& out,
                                   const Input& params) {
  // обработка начисления процентов
  if (!data.percentsDate.empty() && current == data.percentsDate.front()) {
    data.havingRecord = true;

    if (params.capitalisation) {
      // обновление записи отчета и остатка вклада при капитализации
      updateReportRecord(add, data.percents, data.havingRecord);
      out.remainderDeposit += data.percents;
    }

    // обновление записи отчета
    add.percents += data.percents;
    data.percents = 0;
    data.percentsDate.erase(data.percentsDate.begin());
  }
}

/**
 * @brief Processes the additions for the current day and updates the
 * calculation data and output.
 *
 * @param current The current date being processed.
 * @param add The report record to be updated.
 * @param data Calculation data to be updated.
 * @param out Output parameters to be updated.
 * @param additions Vector of addition changes.
 */
void DepositModel::processAdditions(const TimePoint& current, Report& add,
                                    CalcData& data, Output& out,
                                    ChangeVector& additions) {
  // обработка пополнений
  processChangeRecord(current, add, data, out, additions, true);
}

/**
 * @brief Processes the payments for the current day and updates the calculation
 * data and output.
 *
 * @param current The current date being processed.
 * @param add The report record to be updated.
 * @param data Calculation data to be updated.
 * @param out Output parameters to be updated.
 * @param payments Vector of payment changes.
 */
void DepositModel::processPayments(const TimePoint& current, Report& add,
                                   CalcData& data, Output& out,
                                   ChangeVector& payments) {
  // обработка снятий
  processChangeRecord(current, add, data, out, payments, false);
}

/**
 * @brief Calculates the interest and tax for the current day
 * and updates the calculation data and output.
 *
 * @param current The current date being processed.
 * @param data Calculation data to be updated.
 * @param out Output parameters to be updated.
 * @param params Input parameters for the deposit calculation.
 */
void DepositModel::calculateInterestAndTax(const TimePoint& current,
                                           CalcData& data, Output& out,
                                           const Input& params) {
  // расчет процентов за день
  double interest = depositProfitForDay(out.remainderDeposit, params.rate,
                                        daysInYear(current));
  data.percents += interest;
  out.totalPercents += interest;
  data.percentsByYear += interest;

  // расчет налогов, если текущий день - первый день года
  if (isFirstDayOfYear(current)) {
    out.totalTax += calculateTax(&data.percentsByYear, params.taxRate,
                                 params.maxNonTaxableIncome);
  }
}

/******************************************************************************
 * UPDATING THE REPORT
 ******************************************************************************/

/**
 * @brief Adds a report record to the output if there are changes.
 *
 * @param out Output parameters to be updated.
 * @param add The report record to be added.
 * @param havingRecord Flag indicating if there are changes.
 */
void DepositModel::addToReport(Output& out, const Report& add,
                               bool havingRecord) {
  // добавление записи в отчет, если есть изменения
  if (havingRecord) {
    out.report.push_back(add);
  }
}

/**
 * @brief Updates the report record with the addition and remainder.
 *
 * @param add The report record to be updated.
 * @param addition The amount to be added.
 * @param havingRecord Flag indicating if there are changes.
 */
void DepositModel::updateReportRecord(Report& add, double addition,
                                      bool& havingRecord) {
  // обновление записи отчета
  add.addition += addition;
  add.remainder += addition;
  havingRecord = true;
}

/******************************************************************************
 * PROCESSING CHANGES IN CONTRIBUTION
 ******************************************************************************/

/**
 * @brief Processes a change record for the current day and updates the
 * calculation data and output.
 *
 * @param current The current date being processed.
 * @param add The report record to be updated.
 * @param data Calculation data to be updated.
 * @param out Output parameters to be updated.
 * @param changes Vector of change records.
 * @param isAddition Flag indicating if the change is an addition.
 */
void DepositModel::processChangeRecord(const TimePoint& current, Report& add,
                                       CalcData& data, Output& out,
                                       ChangeVector& changes, bool isAddition) {
  // обработка записи изменения
  if (hasChanges(changes, current)) {
    if (isValidChange(changes.front(), isAddition, out.remainderDeposit)) {
      // обработка корректной записи изменения
      processValidChange(add, data, out, changes, isAddition);
    } else {
      // обработка некорректной записи изменения
      processInvalidChange(changes, isAddition);
    }
  }
}

/**
 * @brief Checks if there are changes for the current date.
 *
 * @param changes Vector of change records.
 * @param current The current date being processed.
 * @return True if there are changes for the current date, false otherwise.
 */
bool DepositModel::hasChanges(const ChangeVector& changes,
                              const TimePoint& current) {
  // проверка наличия изменений на текущую дату
  return !changes.empty() && current == changes.front().date;
}

/**
 * @brief Checks if a change record is valid.
 *
 * @param change The change record to be checked.
 * @param isAddition Flag indicating if the change is an addition.
 * @param remainderDeposit The current remainder of the deposit.
 * @return True if the change record is valid, false otherwise.
 */
bool DepositModel::isValidChange(const Change& change, bool isAddition,
                                 double remainderDeposit) {
  // проверка корректности записи изменения
  return (isAddition && change.sum > 0) ||
         (!isAddition && remainderDeposit - change.sum >= 0);
}

/**
 * @brief Processes a valid change record and updates the calculation data and
 * output.
 *
 * @param add The report record to be updated.
 * @param data Calculation data to be updated.
 * @param out Output parameters to be updated.
 * @param changes Vector of change records.
 * @param isAddition Flag indicating if the change is an addition.
 */
void DepositModel::processValidChange(Report& add, CalcData& data, Output& out,
                                      ChangeVector& changes, bool isAddition) {
  // обработка корректной записи изменения (пополнение или снятие средств)
  double changeSum = isAddition ? changes.front().sum : -changes.front().sum;
  updateReportRecord(add, changeSum, data.havingRecord);
  out.remainderDeposit += changeSum;
  changes.erase(changes.begin());
}

/**
 * @brief Processes an invalid change record and logs an error message.
 *
 * @param changes Vector of change records.
 * @param isAddition Flag indicating if the change is an addition.
 */
void DepositModel::processInvalidChange(ChangeVector& changes,
                                        bool isAddition) {
  // обработка некорректной записи изменения
  std::cerr << (isAddition ? "Error: Addition sum must be positive."
                           : "Error: Insufficient funds for payment.")
            << std::endl;
  changes.erase(changes.begin());
}

/******************************************************************************
 * AUXILIARY METHODS
 ******************************************************************************/

/**
 * @brief Calculates the deposit profit for a day.
 *
 * @param deposit The current deposit amount.
 * @param rate The interest rate.
 * @param days_in_year The number of days in the year.
 * @return The deposit profit for the day.
 */
double DepositModel::depositProfitForDay(double deposit, double rate,
                                         int days_in_year) {
  // расчет дохода за день
  return (deposit * rate / days_in_year) / 100;
}

/**
 * @brief Calculates the tax for the given sum.
 *
 * @param sum Pointer to the sum for which the tax is to be calculated.
 * @param rate The tax rate.
 * @param maxNonTaxableIncome The maximum non-taxable income.
 * @return The calculated tax.
 */
double DepositModel::calculateTax(double* sum, double rate,
                                  double maxNonTaxableIncome) {
  // расчет налога
  double border = maxNonTaxableIncome * rate;
  double res = 0;
  if (*sum > border) {
    res = ((*sum - border) * 13) / 100;
  }
  *sum = 0;
  return res;
}

/**
 * @brief Determines the number of days in the year for the given date.
 *
 * @param date The date for which the number
 * of days in the year is to be determined.
 * @return The number of days in the year.
 */
int DepositModel::daysInYear(const TimePoint& date) {
  // определение количества дней в году
  auto timeinfo = convertToTm(date);
  int year = timeinfo->tm_year + 1900;
  if (isLeapYear(year)) {
    return 366;
  } else {
    return 365;
  }
}

/**
 * @brief Checks if the given date is the first day of the year.
 *
 * @param date The date to be checked.
 * @return True if the date is the first day of the year, false otherwise.
 */
bool DepositModel::isFirstDayOfYear(const TimePoint& date) {
  // проверка, является ли дата первым днем года
  auto timeinfo = convertToTm(date);
  return timeinfo->tm_mon == 0 && timeinfo->tm_mday == 1;
}

/**
 * @brief Determines the number of days in the given month and year.
 *
 * @param month The month for which the number of days is to be determined.
 * @param year The year for which the number of days is to be determined.
 * @return The number of days in the month.
 */
int DepositModel::getDaysInMonth(int month, int year) {
  // определение количества дней в месяце
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
 * @brief Checks if the given year is a leap year.
 *
 * @param year The year to be checked.
 * @return True if the year is a leap year, false otherwise.
 */
bool DepositModel::isLeapYear(int year) {
  // проверка, является ли год високосным
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

/**
 * @brief Converts the given date to a tm structure.
 *
 * @param date The date to be converted.
 * @return A pointer to the tm structure representing the date.
 */
std::tm* DepositModel::convertToTm(const TimePoint& date) {
  // преобразование даты в структуру tm
  auto time_t_date = std::chrono::system_clock::to_time_t(date);
  return std::localtime(&time_t_date);
}

/**
 * @brief Determines the number of days in the month for the given date.
 *
 * @param date The date for which the number of days
 * in the month is to be determined.
 * @return The number of days in the month.
 */
int DepositModel::getDaysInMonthFromDate(const TimePoint& date) {
  // определение количества дней от заданной даты
  // до той же даты следующего месяца
  auto timeinfo = convertToTm(date);

  // месяцы в tm_mon начинаются с 0
  int month = timeinfo->tm_mon + 1;
  int year = timeinfo->tm_year + 1900;
  return getDaysInMonth(month, year);
}

}  // namespace s21
