#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  // создаем объект приложения QApplication
  QApplication a(argc, argv);

  // создаем объект главного окна
  MainWindow w;

  // отображаем главное окно
  w.show();

  // запускаем цикл обработки событий приложения
  return a.exec();
}
