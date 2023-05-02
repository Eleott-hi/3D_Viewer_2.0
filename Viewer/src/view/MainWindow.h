#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>

#include "Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(Backend *backend, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_actionOpen_file_triggered();

 private:
  Ui::MainWindow *ui_;
  Backend *backend_;

 protected:
  virtual void keyPressEvent(QKeyEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;
};

}  // namespace s21
#endif  // MAINWINDOW_H
