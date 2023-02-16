#include <QApplication>

#include "backend.h"
#include "controller.h"
#include "main_window.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::Backend model;
  s21::Controller controller(&model);
  s21::MainWindow window(&controller);

  window.show();

  return a.exec();
}
