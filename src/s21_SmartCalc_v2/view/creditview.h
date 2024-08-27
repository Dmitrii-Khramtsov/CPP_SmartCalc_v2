#ifndef CREDITVIEW_H
#define CREDITVIEW_H

#include <QMainWindow>
#include "../controller/calc_controller.h"

namespace Ui {
class CreditView;
}

class CreditView : public QMainWindow {
  Q_OBJECT

 public:
  explicit CreditView(QWidget *parent = nullptr);
  ~CreditView();

 private slots:
  void calc_credit();

 private:
  Ui::CreditView *ui;
  s21::CalcController controller;

  s21::CrInput initializeCreditConditions();
  void updateCreditTable(const s21::PaymentVector& payment_vector);
  void updateCreditLabels(double monthly_pay, const s21::CrOutput& output);
};

#endif // CREDITVIEW_H
