// Copyright 2024 Dmitrii Khramtsov

/**
 * @file creditview.cpp
 *
 * @brief Implementation of the CreditView class for the SmartCalc v2.0 application.
 *
 * This file contains the implementation of the CreditView class, which handles the user interface and interactions for credit calculations.
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-08-23
 *
 * @copyright School-21 (c) 2024
 */

#include "creditview.h"
#include "ui_creditview.h"

/**
 * @brief Constructor for the CreditView class.
 *
 * @param parent Pointer to the parent widget.
 */
CreditView::CreditView(QWidget *parent) :
                                          QMainWindow(parent),
                                          ui(new Ui::CreditView) {
  ui->setupUi(this);

  // устанавливаем название окна
  setWindowTitle("CreditCalc");

  // подключение сигналов от кнопок к слотам
  connect(ui->pushButton_credit, &QPushButton::clicked, this, &CreditView::calc_credit);
}

/**
 * @brief Destructor for the CreditView class.
 */
CreditView::~CreditView() {
  delete ui;
}

/**
 * @brief Calculates the credit based on the user input.
 */
void CreditView::calc_credit() {
  s21::CrInput conditions = initializeCreditConditions();
  s21::CrOutput output;
  double monthly_pay = 0;
  s21::PaymentVector payment_vector;

  try {
    // вычисляем кредитные условия и получаем результаты
    controller.calculateCredit(static_cast<s21::TypeOfMonthlyPayments>(ui->comboBox->currentIndex()), conditions, monthly_pay, output, payment_vector);
    // обновляем таблицу кредита
    updateCreditTable(payment_vector);
    // обновляем метки кредита
    updateCreditLabels(monthly_pay, output);
  } catch (...) {
    // отображаем сообщение об ошибке в статусной строке
    ui->statusBar->showMessage("Не корректно введены данные для расчета кредита", 3000);
  }
}

/**
 * @brief Initializes the credit conditions based on the user input.
 *
 * @return The initialized credit conditions.
 */
s21::CrInput CreditView::initializeCreditConditions() {
  s21::CrInput conditions = {0};
  // инициализируем условия кредита
  conditions.credit = ui->doubleSpinBox_credit_summ->value();
  conditions.term = ui->spinBox_number_of_mouth->value();
  conditions.rate = ui->doubleSpinBox_interest_rate->value();
  return conditions;
}

/**
 * @brief Updates the credit table with the calculation results.
 *
 * @param payment_vector The vector containing the payment data.
 */
void CreditView::updateCreditTable(const s21::PaymentVector& payment_vector) {
  // устанавливаем количество столбцов и строк в таблице
  ui->tableCredit->setColumnCount(5); // предполагается 5 столбцов: Date, Monthly Payment, Total Payment, Total Reminder, Const Payment
  ui->tableCredit->setRowCount(payment_vector.size());

  // заполняем таблицу данными из payment_vector
  for (int row = 0; row < payment_vector.size(); row++) {
    const s21::Payment& payment = payment_vector[row];

    QTableWidgetItem *itemDate = new QTableWidgetItem();
    itemDate->setTextAlignment(Qt::AlignRight);
    itemDate->setText(QString::fromStdString(payment.date));
    ui->tableCredit->setItem(row, 0, itemDate);

    QTableWidgetItem *itemMonthlyPay = new QTableWidgetItem();
    itemMonthlyPay->setTextAlignment(Qt::AlignRight);
    itemMonthlyPay->setText(QString::number(payment.monthly_pay, 'f', 2));
    ui->tableCredit->setItem(row, 1, itemMonthlyPay);

    QTableWidgetItem *itemTotal = new QTableWidgetItem();
    itemTotal->setTextAlignment(Qt::AlignRight);
    itemTotal->setText(QString::number(payment.interest_pay, 'f', 2));
    ui->tableCredit->setItem(row, 2, itemTotal);

    QTableWidgetItem *itemTotalReminder = new QTableWidgetItem();
    itemTotalReminder->setTextAlignment(Qt::AlignRight);
    itemTotalReminder->setText(QString::number(payment.const_payment, 'f', 2));
    ui->tableCredit->setItem(row, 3, itemTotalReminder);

    QTableWidgetItem *itemConstPayment = new QTableWidgetItem();
    itemConstPayment->setTextAlignment(Qt::AlignRight);
    itemConstPayment->setText(QString::number(payment.total_reminder, 'f', 2));
    ui->tableCredit->setItem(row, 4, itemConstPayment);
  }

  // изменяем размер столбцов по содержимому
  ui->tableCredit->resizeColumnsToContents();
}

/**
 * @brief Updates the credit labels with the calculation results.
 *
 * @param monthly_pay The monthly payment amount.
 * @param output The output data containing the calculation results.
 */
void CreditView::updateCreditLabels(double monthly_pay, const s21::CrOutput& output) {
  // обновляем метки с данными о кредите
  ui->label_overpayment_monthly_payment->setText(QString::number(monthly_pay, 'f', 2));
  ui->label_overpayment->setText(QString::number(output.overpayment, 'f', 2));
  ui->label_overpayment_total_payment->setText(QString::number(output.total, 'f', 2));
}
