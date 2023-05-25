#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>

#include "Observer.h"
#include "Scene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow, protected Observer {
  Q_OBJECT

 public:
  MainWindow(Scene *scene, QWidget *parent = nullptr);
  ~MainWindow();

  void OnNotify() final;
  void OnCameraNotify() final;

 private slots:
  void on_actionOpen_file_triggered();

 private:
  Scene *scene_;
  Ui::MainWindow *ui_;

 protected:
  virtual void keyPressEvent(QKeyEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;

  void ConnectSignals();
  void SetTransformUi(Transform const &component);
  void SetLightUi(Light const &component);
  void SetMaterialUi(Material const &component);
  void SetShaderUi(Shader const &component);
  void ConnectTransformUi();
  void ConnectLightUi();
  void ConnectMaterialUi();
  void ConnectShaderUi();
  void ConnectCameraUi();
};

}  // namespace s21
#endif  // MAINWINDOW_H
