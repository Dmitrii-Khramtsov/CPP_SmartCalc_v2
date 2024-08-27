#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QMainWindow>
#include "../controller/calc_controller.h"

namespace Ui {
class GraphView;
}

class GraphView : public QMainWindow {
  Q_OBJECT

 public:
  explicit GraphView(QWidget *parent = nullptr);
  ~GraphView();

 private slots:
  void build_graf();

 private:
  Ui::GraphView *ui;
  s21::CalcController controller;
};

#endif  // GRAPHVIEW_H
