#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>

#include "Backend.h"
#include "Observer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow, protected Observer {
  Q_OBJECT

 public:
  MainWindow(Backend *backend, QWidget *parent = nullptr);
  ~MainWindow();

  void OnNotify() final;

 private slots:
  void on_actionOpen_file_triggered();

 private:
  Backend *backend_;
  Ui::MainWindow *ui_;

 protected:
  virtual void keyPressEvent(QKeyEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;

  void ConnectSignals();
  void SetTransformUi(Transform const &transform);
  void SetLightUi(Light const &transform);
  void SetMaterialUi(Material const &transform);
  void ConnectTransformUi();
  void ConnectLightUi();
  void ConnectMaterialUi();
};

}  // namespace s21
#endif  // MAINWINDOW_H
