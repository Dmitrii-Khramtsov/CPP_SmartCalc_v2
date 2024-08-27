// Copyright 2024 Dmitrii Khramtsov

/**
 * @file depositview.cpp
 *
 * @brief Implementation of the DepositView class for the SmartCalc v2.0 application.
 *
 * This file contains the implementation of the DepositView class, which handles the user interface and interactions for deposit calculations.
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-08-12
 *
 * @copyright School-21 (c) 2024
 */

#include "depositview.h"
#include "ui_depositview.h"

/**
 * @brief Constructor for the DepositView class.
 *
 * @param parent Pointer to the parent widget.
 */
DepositView::DepositView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DepositView) {
  ui->setupUi(this);

  // устанавливаем название окна
  setWindowTitle("DepositCalc");

  // подключение сигналов от кнопок к слотам
  connect(ui->pushButton_deposit, &QPushButton::clicked, this, &DepositView::calc_deposit);
}

/**
 * @brief Destructor for the DepositView class.
 */
DepositView::~DepositView() {
  delete ui;
}

/**
 * @brief Calculates the deposit based on the user input.
 */
void DepositView::calc_deposit() {
  s21::Input conditions = initializeDepositConditions();
  s21::Output output;

  try {
    // вычисляем условия депозита и получаем результаты
    controller.calculateDeposit(conditions, output);
    // обновляем таблицу депозита
    updateDepositTable(output);
    // обновляем метки депозита
    updateDepositLabels(output);
  } catch (...) {
    // отображаем сообщение об ошибке в статусной строке
    ui->statusbar->showMessage("Не корректно введены данные для расчета вклада", 3000);
  }
}

/**
 * @brief Initializes the deposit conditions based on the user input.
 *
 * @return The initialized deposit conditions.
 */
s21::Input DepositView::initializeDepositConditions() {
  s21::Input conditions = {0};
  // инициализируем условия депозита
  conditions.deposit = ui->doubleSpinBox_deposit_amount->value();
  conditions.monthsTerm = ui->spinBox_period_of_placement->value();
  conditions.rate = ui->doubleSpinBox_interest_rate_d->value();
  conditions.taxRate = ui->doubleSpinBox_tax_rate->value();
  conditions.capitalisation = ui->radioButton_add->isChecked();
  conditions.now = std::chrono::system_clock::now();
  handleAdditions(conditions);
  handlePayments(conditions);
  conditions.maxNonTaxableIncome = 10000;
  conditions.period = static_cast<s21::Period>(ui->comboBox_period_pays->currentIndex());
  return conditions;
}

/**
 * @brief Updates the deposit table with the calculation results.
 *
 * @param output The output data containing the calculation results.
 */
void DepositView::updateDepositTable(const s21::Output& output) {
  // устанавливаем количество столбцов и строк в таблице
  ui->tableDeposit->setColumnCount(4); // предполагается 4 столбца: Date, Payment, Refilling, Deposit
  ui->tableDeposit->setRowCount(output.report.size());

  // заполняем таблицу данными из output.report
  for (size_t i = 0; i < output.report.size(); ++i) {
    const auto& report = output.report[i];
    auto time_t_date = std::chrono::system_clock::to_time_t(report.date);
    auto timeinfo = std::localtime(&time_t_date);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);

    ui->tableDeposit->setItem(i, 0, new QTableWidgetItem(buffer)); // Date
    ui->tableDeposit->setItem(i, 1, new QTableWidgetItem(QString::number(report.percents, 'f', 2))); // Payment
    ui->tableDeposit->setItem(i, 2, new QTableWidgetItem(QString::number(report.addition, 'f', 2))); // Refilling
    ui->tableDeposit->setItem(i, 3, new QTableWidgetItem(QString::number(report.remainder, 'f', 2))); // Deposit
  }
}

/**
 * @brief Updates the deposit labels with the calculation results.
 *
 * @param output The output data containing the calculation results.
 */
void DepositView::updateDepositLabels(const s21::Output& output) {
  // обновляем метки с данными о депозите
  ui->label_interest_charges->clear();
  ui->label_interest_charges->setText(QString::number(output.totalPercents, 'f', 2));
  ui->label_tax_amount->clear();
  ui->label_tax_amount->setText(QString::number(output.totalTax, 'f', 2));
  ui->label_deposit_amount->clear();
  ui->label_deposit_amount->setText(QString::number(output.remainderDeposit, 'f', 2));
}

/**
 * @brief Handles the additions to the deposit based on the selected interval.
 *
 * @param conditions The deposit conditions to be updated.
 */
void DepositView::handleAdditions(s21::Input& conditions) {
  // обрабатываем добавления в зависимости от выбранного интервала
  switch (ui->combobox_deposit_addpay->currentIndex()) {
    case 0: conditions.additions.push_back({conditions.now, 0}); break;
    case 1: handlePeriodicAddition(conditions, 30); break;
    case 2: handlePeriodicAddition(conditions, 90); break;
    case 3: handlePeriodicAddition(conditions, 180); break;
    case 4: handlePeriodicAddition(conditions, 365); break;
    default: break;
  }
}

/**
 * @brief Handles periodic additions to the deposit.
 *
 * @param conditions The deposit conditions to be updated.
 * @param days The number of days for the periodic addition.
 */
void DepositView::handlePeriodicAddition(s21::Input& conditions, int days) {
  double additionSum = ui->doubleSpinBox_add_sum->value();
  if (additionSum > 0) {
    // добавляем периодические пополнения
    for (int i = 1; i <= conditions.monthsTerm; ++i) {
      conditions.additions.push_back({conditions.now + std::chrono::hours(24 * days * i), additionSum});
    }
  } else {
    qDebug() << "Ошибка: Сумма пополнения должна быть положительной.";
    ui->statusbar->showMessage("Сумма пополнения должна быть положительной.", 3000);
  }
}

/**
 * @brief Handles the payments from the deposit based on the selected interval.
 *
 * @param conditions The deposit conditions to be updated.
 */
void DepositView::handlePayments(s21::Input& conditions) {
  // обрабатываем снятия в зависимости от выбранного интервала
  switch (ui->combobox_deposit_snyatie->currentIndex()) {
    case 0: conditions.payments.push_back({conditions.now, 0}); break;
    case 1: handlePeriodicPayment(conditions, 30); break;
    case 2: handlePeriodicPayment(conditions, 90); break;
    case 3: handlePeriodicPayment(conditions, 180); break;
    case 4: handlePeriodicPayment(conditions, 365); break;
    default: break;
  }
}

/**
 * @brief Handles periodic payments from the deposit.
 *
 * @param conditions The deposit conditions to be updated.
 * @param days The number of days for the periodic payment.
 */
void DepositView::handlePeriodicPayment(s21::Input& conditions, int days) {
  double paymentSum = ui->doubleSpinBox_withdrawals->value();
  if (paymentSum > 0) {
    // добавляем периодические снятия
    for (int i = 1; i <= conditions.monthsTerm; ++i) {
      conditions.payments.push_back({conditions.now + std::chrono::hours(24 * days * i), paymentSum});
    }
  } else {
    qDebug() << "Ошибка: Сумма снятия должна быть положительной.";
    ui->statusbar->showMessage("Сумма снятия должна быть положительной.", 3000);
  }
}
