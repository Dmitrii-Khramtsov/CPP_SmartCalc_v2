#ifndef DEPOSITVIEW_H
#define DEPOSITVIEW_H

#include <QMainWindow>
#include "../controller/calc_controller.h"

namespace Ui {
class DepositView;
}

class DepositView : public QMainWindow {
  Q_OBJECT

 public:
  explicit DepositView(QWidget *parent = nullptr);
  ~DepositView();

 private slots:
  void calc_deposit();

 private:
  Ui::DepositView *ui;
  s21::CalcController controller;

  s21::Input initializeDepositConditions();
  void updateDepositTable(const s21::Output& output);
  void updateDepositLabels(const s21::Output& output);

  void handleAdditions(s21::Input& conditions);
  void handlePeriodicAddition(s21::Input& conditions, int days);
  void handlePayments(s21::Input& conditions);
  void handlePeriodicPayment(s21::Input& conditions, int days);
};

#endif  // DEPOSITVIEW_H
