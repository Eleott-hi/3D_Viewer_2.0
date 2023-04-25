#pragma once

#include <QMainWindow>
#include <QVector3D>

#include "controller.h"
#include "signal_handler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(Controller *c, QWidget *parent = nullptr);
  ~MainWindow();

  void keyPressEvent(QKeyEvent *event);
  void ConnectCameraSignals();
  void ConnectObjectSignals();
  void ConnectLightSignals();

 private slots:
  void on_actionOpen_file_triggered();

  void SetStackCamera();
  void on_btn_ResetCamera_clicked();
  void on_btn_BackColor_clicked();
  void SetStackObject();
  void SetInfoPanel(QString const &file, uint32_t vertices, uint32_t indices);
  void SetTransformPanel(QVector3D position, QVector3D rotation, float scale);
  void SetPointPanel(bool show, bool smooth, int size, QColor color);
  void SetLinePanel(bool show, bool dashed, int size, QColor color);
  void SetTexturePanel(int index);
  void SetMaterialPanel(float shininess);
  void SetPointLightPanel(bool sourceLight,           //
                          QVector3D const &ambient,   //
                          QVector3D const &diffuse,   //
                          QVector3D const &specular,  //
                          QVector3D const &constants);
  void SetAxisPanel(bool value);
  void UpdateTransformInfo();
  void on_ShowVertex_toggled(bool checked);
  void on_PointSize_2_valueChanged(int value);
  void on_SmoothPoints_2_toggled(bool value);
  void on_DotsColor_2_clicked();
  void on_showLines_toggled(bool checked);
  void on_DashedLines_2_toggled(bool checked);
  void on_LineWidth_2_valueChanged(int value);
  void on_LinesColor_2_clicked();
  void on_cb_RenderTechnique_currentIndexChanged(int index);
  void on_btn_LoadTexture_clicked();
  void on_btn_ResetObject_clicked();
  void on_tg_LightSource_toggled(bool checked);
  void UpdatePointLightInfo();



  // Camera
  void SetCameraPanel(QVector3D position, QVector3D rotation, float zoom);
  void UpdateCameraInfo();
  void on_Shininess_valueChanged(int value);

private:
  Ui::MainWindow *ui_;
  Controller *controller_;
  CommonSettings &cSettings_ = CommonSettings::GetInstance();
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();

  void ConnectSignals();
};
}  // namespace s21
