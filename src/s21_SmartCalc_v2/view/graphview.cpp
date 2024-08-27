// Copyright 2024 Dmitrii Khramtsov

/**
 * @file graphview.cpp
 *
 * @brief Implementation of the GraphView class for the SmartCalc v2.0 application.
 *
 * This file contains the implementation of the GraphView class, which handles the user interface and interactions for graph calculations.
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-08-12
 *
 * @copyright School-21 (c) 2024
 */

#include "graphview.h"
#include "ui_graphview.h"

/**
 * @brief Constructor for the GraphView class.
 *
 * @param parent Pointer to the parent widget.
 */
GraphView::GraphView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GraphView) {
  ui->setupUi(this);

  // устанавливаем название окна
  setWindowTitle("Graph");

  // устанавливаем отступы для QLineEdit
  ui->lineEdit->setStyleSheet("QLineEdit { padding: 10px; }");

  // подключение сигналов от кнопок к слотам
  connect(ui->pushButton_graph, &QPushButton::clicked, this, &GraphView::build_graf);
}

/**
 * @brief Destructor for the GraphView class.
 */
GraphView::~GraphView() {
  delete ui;
}

/**
 * @brief Builds and displays the graph based on user input.
 */
void GraphView::build_graf() {
  double vXMin = ui->doubleSpinBox_Xmin->value();
  double vXMax = ui->doubleSpinBox_Xmax->value();
  double vYMin = ui->doubleSpinBox_Ymin->value();
  double vYMax = ui->doubleSpinBox_Ymax->value();
  QVector<double> x, y;
  std::vector<std::vector<double>> answer;

  // проверяем, не пустой ли текст в QLineEdit
  if (ui->lineEdit->text() != "") {
    try {
      // вычисляем данные для графика
      answer = controller.calculateGraf(std::make_pair(vXMin, vXMax),
                                        std::make_pair(vYMin, vYMax),
                                        ui->spinBox_points->value(),
                                        ui->lineEdit->text().toStdString());
      // заполняем векторы x и y данными из answer
      for (int i = 0; i < ui->spinBox_points->value(); i++) {
        x.push_back(answer[0][i]);
        y.push_back(answer[1][i]);
      }
    } catch (std::exception const &errorMessage) {
      // отображаем сообщение об ошибке в статусной строке
      ui->statusbar->showMessage(
          "ОШИБКА: возможно " + QString::fromStdString(errorMessage.what()),
          3000);
    }

    // очищаем графики и добавляем новый график
    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);

    // устанавливаем стиль линии и маркеров в зависимости от выбранного индекса в comboBox_2
    if (ui->comboBox_2->currentIndex()) {
      ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
      ui->widget->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    } else {
      ui->widget->graph(0)->setLineStyle(QCPGraph::lsLine);
    }

    // устанавливаем диапазоны осей
    ui->widget->xAxis->setRange(vXMin, vXMax);
    ui->widget->yAxis->setRange(vYMin, vYMax);

    // перерисовываем график
    ui->widget->replot();

    // включаем взаимодействие с графиком (масштабирование и перетаскивание)
    ui->widget->setInteraction(QCP::iRangeZoom, true);
    ui->widget->setInteraction(QCP::iRangeDrag, true);
  }
}
