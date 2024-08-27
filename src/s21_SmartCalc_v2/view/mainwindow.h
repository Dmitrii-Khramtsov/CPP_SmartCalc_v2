#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QWidget>
#include "../controller/calc_controller.h"
#include "../view/creditview.h"
#include "../view/depositview.h"
#include "../view/graphview.h"  // Добавляем заголовочный файл GraphView

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  s21::CalcController controller;
  Ui::MainWindow *ui;
  CreditView *creditView;  // Declare the creditView member variable
  DepositView *depositView;  // Declare the depositView member variable
  GraphView *graphView;  // Declare the graphView member variable

 private slots:
  void digits_numbers();
  void operations();
  void functions();
  void on_pushButton_dot_clicked();
  void on_pushButton_AC_clicked();
  void on_pushButton_C_clicked();
  void on_pushButton_sqrt_clicked();
  void on_pushButton_X_clicked();
  void on_pushButton_eqv_clicked();
  void on_lineEdit_textEdited(const QString &arg1);

  void showCreditView();  // Declare the showCreditView slot
  void showDepositView();  // Declare the showDepositView slot
  void showGraphView();  // Declare the showGraphView slot
};

#endif  // MAINWINDOW_H
