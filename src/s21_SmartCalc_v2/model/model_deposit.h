// Copyright 2024 Dmitrii Khramtsov

/**
 * @file model_deposit.h
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


#ifndef CPP3_S21_SMART_CALC_MODEL_DEPOSIT_H
#define CPP3_S21_SMART_CALC_MODEL_DEPOSIT_H

#include <iostream>
#include <algorithm>  // sortChangeRecords
#include <chrono>     // calculateDeposit, processDay, initPercentsVector
#include <cmath>      // depositProfitForDay
#include <ctime>      // daysInYear, isFirstDayOfYear, convertToTm
#include <vector>     // sortChangeRecords, processChangeRecord

namespace s21 {

using TimePoint = std::chrono::system_clock::time_point;

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

// добавление или снятие средств
struct Change {
  TimePoint date;  // дата изменения
  double sum;      // сумма изменения
};

// запись отчета о состоянии депозита
struct Report {
  TimePoint date;    // дата отчета
  double percents;   // проценты
  double addition;   // добавление
  double remainder;  // остаток
};

enum class Period { EveryDay, EveryMonth, EveryYear };

// изначальные входные данные
struct Input {
  using ChangeVector = std::vector<Change>;

  double deposit;              // сумма вклада
  double rate;                 // годовая процентная ставка по вкладу
  double taxRate;              // ставка центрального банка
  int monthsTerm;              // срок (в месяцах) размещения вклада
  bool capitalisation;         // капитализация есть = 1 или нет = 0
  Period period;               // единица измерения срока вклада (день, месяц, год)
  ChangeVector additions;      // массив частичных пополнений вклада
  ChangeVector payments;       // массив частичных снятий вклада
  TimePoint now;               // дата начала вклада
  double maxNonTaxableIncome;  // максимальный доход, не облагаемый налогом НДФЛ
};

// выходные данные - резульраты расчётов
struct Output {
  std::vector<Report> report;  // отчет
  double totalPercents;        // общие проценты
  double totalTax;             // общий налог
  double remainderDeposit;     // остаток депозита
};

// данные для расчетов
struct CalcData {
  using TimePointVector = std::vector<TimePoint>;

  TimePointVector percentsDate;  // даты начисления процентов
  TimePoint end;                 // дата окончания вклада
  bool havingRecord;             // флаг наличия записи
  double percents;               // проценты
  double percentsByYear;         // проценты за год
};

class DepositModel {
 public:
  using ChangeVector = std::vector<Change>;
  using ReportVector = std::vector<Report>;
  using TimePoint = std::chrono::system_clock::time_point;
  using TimePointVector = std::vector<TimePoint>;

  DepositModel();

  // основной метод
  void calculateDeposit(const Input& in, Output& out);

 private:
  // инициализация данных
  void initializeOutput(Output& out, const Input& in);
  void updateInitialReportRecord(Output& out, const Input& in);
  void initializeCalculationData(CalcData& data, const Input& params,
                                 ChangeVector& additions,
                                 ChangeVector& payments);
  void sortChangeRecords(ChangeVector& changes);
  void initPercentsVector(CalcData& data, const Input& params);

  // обработка дней и изменений
  void processDay(const TimePoint& current, CalcData& data, Output& out,
                  const Input& params, ChangeVector& additions,
                  ChangeVector& payments);
  void processInterest(const TimePoint& current, Report& add, CalcData& data,
                       Output& out, const Input& params);
  void processAdditions(const TimePoint& current, Report& add, CalcData& data,
                        Output& out, ChangeVector& additions);
  void processPayments(const TimePoint& current, Report& add, CalcData& data,
                       Output& out, ChangeVector& payments);
  void calculateInterestAndTax(const TimePoint& current, CalcData& data,
                               Output& out, const Input& params);

  // обновление отчета
  void addToReport(Output& out, const Report& add, bool havingRecord);
  void updateReportRecord(Report& add, double addition, bool& havingRecord);

  // обработка изменений вклада
  void processChangeRecord(const TimePoint& current, Report& add,
                           CalcData& data, Output& out, ChangeVector& changes,
                           bool isAddition);
  bool hasChanges(const ChangeVector& changes, const TimePoint& current);
  bool isValidChange(const Change& change, bool isAddition,
                     double remainderDeposit);
  void processValidChange(Report& add, CalcData& data, Output& out,
                          ChangeVector& changes, bool isAddition);
  void processInvalidChange(ChangeVector& changes, bool isAddition);

  // вспомогательные методы
  double depositProfitForDay(double deposit, double rate, int days_in_year);
  double calculateTax(double* sum, double rate, double maxNonTaxableIncome);
  int daysInYear(const TimePoint& date);
  bool isFirstDayOfYear(const TimePoint& date);
  int getDaysInMonth(int month, int year);
  bool isLeapYear(int year);
  std::tm* convertToTm(const TimePoint& date);
  int getDaysInMonthFromDate(const TimePoint& date);
};

}  // namespace s21

#endif  // CPP3_S21_SMART_CALC_MODEL_DEPOSIT_H
