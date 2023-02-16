#include <QApplication>

#include "../headers/backend.h"
#include "../headers/controller.h"
#include "../headers/main_window.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::Backend model;
  s21::Controller controller(&model);
  s21::MainWindow w(&controller);
  w.show();
  return a.exec();
}
