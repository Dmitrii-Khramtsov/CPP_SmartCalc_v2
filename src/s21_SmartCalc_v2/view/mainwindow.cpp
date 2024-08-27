// Copyright 2024 Dmitrii Khramtsov

/**
 * @file mainwindow.cpp
 *
 * @brief Implementation of the MainWindow class for the SmartCalc v2.0 application.
 *
 * This file contains the implementation of the MainWindow class, which is the main window of the SmartCalc v2.0 application.
 * The MainWindow class handles the user interface and interactions with the calculator.
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-08-12
 *
 * @copyright School-21 (c) 2024
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../controller/calc_controller.h"
#include "creditview.h"
#include "depositview.h"
#include "graphview.h"
#include <QIcon>

/**
 * @brief Constructor for the MainWindow class.
 *
 * @param parent Pointer to the parent widget.
 */
MainWindow::MainWindow(QWidget *parent) :
                                          QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // устанавливаем название окна
  setWindowTitle("SmartCalc_v2 by Lonmouth");

  // устанавливаем иконку окна
  // QIcon icon(":/SmartCalc_v2.png");
  // setWindowIcon(QIcon("/Users/dmitrii/CPP3_SmartCalc_v2.0-1/src/s21_SmartCalc_v2/build/SmartCalc_v2.png"));
  setWindowIcon(QIcon(":/SmartCalc_v2.ico"));

  // устанавливаем отступы для QLineEdit
  ui->lineEdit->setStyleSheet("QLineEdit { padding: 10px; }");

  // подключаем кнопки цифр к слоту digits_numbers
  connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_1, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::digits_numbers);
  connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::digits_numbers);

  // подключаем кнопки операций к слоту operations
  connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_sub, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_open_bracket, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_close_bracket, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_sqr, SIGNAL(clicked()), this, SLOT(operations()));

  // подключаем кнопки функций к слоту functions
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(functions()));

  // подключаем сигнал нажатия Enter в QLineEdit к слоту on_pushButton_eqv_clicked
  connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(on_pushButton_eqv_clicked()));

  // подключаем кнопки для открытия дополнительных окон к соответствующим слотам
  connect(ui->pushButton_credit_open, SIGNAL(clicked()), this, SLOT(showCreditView()));
  connect(ui->pushButton_deposit_open, SIGNAL(clicked()), this, SLOT(showDepositView()));
  connect(ui->pushButton_graph_open, SIGNAL(clicked()), this, SLOT(showGraphView()));

  // инициализируем и настраиваем дополнительные окна
  creditView = new CreditView(this);
  creditView->setGeometry(0, 0, this->width(), this->height());
  creditView->hide();

  depositView = new DepositView(this);
  depositView->setGeometry(0, 0, this->width(), this->height());
  depositView->hide();

  graphView = new GraphView(this);
  graphView->setGeometry(0, 0, this->width(), this->height());
  graphView->hide();
}

/**
 * @brief Destructor for the MainWindow class.
 */
MainWindow::~MainWindow() {
  delete ui;
}

/**
 * @brief Shows or hides the credit view.
 */
void MainWindow::showCreditView() {
  // проверяем, скрыто ли окно кредита
  if (creditView->isHidden()) {
    // если скрыто, показываем его
    creditView->show();
  } else {
    // если не скрыто, скрываем его
    creditView->hide();
  }
}

/**
 * @brief Shows or hides the deposit view.
 */
void MainWindow::showDepositView() {
  // проверяем, скрыто ли окно депозита
  if (depositView->isHidden()) {
    // если скрыто, показываем его
    depositView->show();
  } else {
    // если не скрыто, скрываем его
    depositView->hide();
  }
}

/**
 * @brief Shows or hides the graph view.
 */
void MainWindow::showGraphView() {
  // проверяем, скрыто ли окно графика
  if (graphView->isHidden()) {
    // если скрыто, показываем его
    graphView->show();
  } else {
    // если не скрыто, скрываем его
    graphView->hide();
  }
}

/**
 * @brief Handles digit button clicks.
 */
void MainWindow::digits_numbers() {
  // получаем указатель на кнопку, которая была нажата
  QPushButton *button = (QPushButton *)sender();
  // вставляем текст кнопки в QLineEdit
  ui->lineEdit->insert(button->text());
}

/**
 * @brief Handles operation button clicks.
 */
void MainWindow::operations() {
  // получаем указатель на кнопку, которая была нажата
  QPushButton *button = (QPushButton *)sender();
  QString new_label;
  new_label = ui->lineEdit->text();

  // проверяем текст кнопки и добавляем соответствующий символ в QLineEdit
  if (button->text() == "÷" && !(ui->lineEdit->text().endsWith("/"))) {
    new_label += "/";
  } else if (button->text() == "*" && !(ui->lineEdit->text().endsWith("*"))) {
    new_label += "*";
  } else if (button->text() == "+" && !(ui->lineEdit->text().endsWith("+"))) {
    new_label += "+";
  } else if (button->text() == "-" && !(ui->lineEdit->text().endsWith("-"))) {
    new_label += "-";
  } else if (button->text() == "^" && !(ui->lineEdit->text().endsWith("^"))) {
    new_label += "^";
  } else if (button->text() == "%" && !(ui->lineEdit->text().endsWith("%"))) {
    new_label += "%";
  } else if (button->text() == "(") {
    new_label += "(";
  } else if (button->text() == ")") {
    new_label += ")";
  }

  // устанавливаем новый текст в QLineEdit
  ui->lineEdit->setText(new_label);
}

/**
 * @brief Handles function button clicks.
 */
void MainWindow::functions() {
  // получаем указатель на кнопку, которая была нажата
  QPushButton *button = (QPushButton *)sender();
  // вставляем текст кнопки и открывающую скобку в QLineEdit
  ui->lineEdit->insert(button->text() + "(");
}

/**
 * @brief Handles the dot button click.
 */
void MainWindow::on_pushButton_dot_clicked() {
  // проверяем, заканчивается ли текст в QLineEdit точкой
  if (!(ui->lineEdit->text().endsWith(".")))
    // если нет, добавляем точку
    ui->lineEdit->setText(ui->lineEdit->text() + ".");
}

/**
 * @brief Handles the AC button click.
 */
void MainWindow::on_pushButton_AC_clicked() {
  // получаем указатель на кнопку, которая была нажата
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  // если кнопка не равна nullptr, удаляем последний символ в QLineEdit
  if (button != nullptr) {
    ui->lineEdit->backspace();
  }
}

/**
 * @brief Handles the C button click.
 */
void MainWindow::on_pushButton_C_clicked() {
  // очищаем текст в QLineEdit
  ui->lineEdit->setText("");
  // очищаем сообщение в статусной строке
  ui->statusbar->showMessage("");
}

/**
 * @brief Handles the sqrt button click.
 */
void MainWindow::on_pushButton_sqrt_clicked() {
  // вставляем текст "sqrt(" в QLineEdit
  ui->lineEdit->insert("sqrt(");
}

/**
 * @brief Handles the X button click.
 */
void MainWindow::on_pushButton_X_clicked() {
  // вставляем текст "x" в QLineEdit
  ui->lineEdit->insert("x");
}

/**
 * @brief Handles the equals button click.
 */
void MainWindow::on_pushButton_eqv_clicked() {
  double vAnswer = 0;
  // проверяем, не пустой ли текст в QLineEdit
  if (ui->lineEdit->text() != "") {
    try {
      // вычисляем выражение и получаем результат
      vAnswer = controller.calculateExpression(ui->lineEdit->text().toStdString(),
                                               ui->doubleSpinBox_X->value());
      // очищаем текст в QLineEdit
      ui->lineEdit->setText("");
      // устанавливаем результат в QLineEdit
      ui->lineEdit->setText(QString::number(vAnswer, 'f', 8));
    } catch (std::exception const &vErrorMessage) {
      // отображаем сообщение об ошибке в статусной строке
      ui->statusbar->showMessage(
          "ОШИБКА: возможно " + QString::fromStdString(vErrorMessage.what()),
          3000);
    }
  }
}

/**
 * @brief Handles the text edited signal of the QLineEdit.
 *
 * @param arg1 The new text in the QLineEdit.
 */
void MainWindow::on_lineEdit_textEdited(const QString &arg1) {
  // проверяем, заканчивается ли текст в QLineEdit символом "="
  if (arg1.endsWith("=", Qt::CaseInsensitive)) {
    // удаляем последний символ в QLineEdit
    ui->lineEdit->backspace();
    // вызываем слот on_pushButton_eqv_clicked
    on_pushButton_eqv_clicked();
  }
}
