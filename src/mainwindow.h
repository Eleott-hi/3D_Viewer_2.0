#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <qgifimage.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void gif_timer();
  void on_ModelColor_clicked();
  void on_File_clicked();
  void on_Screenshot_clicked();
  void on_Gif_clicked();
  void on_xTrans_valueChanged(int value);
  void on_yTrans_valueChanged(int value);
  void on_zTrans_valueChanged(int value);
  void on_xRot_valueChanged(int value);
  void on_yRot_valueChanged(int value);
  void on_zRot_valueChanged(int value);
  void on_Scale_valueChanged(int value);
  void on_PointSize_valueChanged(int value);
  void on_LineWidth_valueChanged(int value);
  void on_xTransDoubleBox_valueChanged(double arg1);
  void on_yTransDoubleBox_valueChanged(double arg1);
  void on_zTransDoubleBox_valueChanged(double arg1);
  void on_xRotDoubleBox_valueChanged(double arg1);
  void on_yRotDoubleBox_valueChanged(double arg1);
  void on_zRotDoubleBox_valueChanged(double arg1);
  void on_ScaleDoubleBox_valueChanged(double arg1);
  void on_Reset_clicked();
  void on_Perspective_toggled(bool checked);
  void on_AxisLines_toggled(bool checked);
  void on_SmoothPoints_toggled(bool checked);
  void on_DashedLines_toggled(bool checked);
  void on_BackgroundColor_clicked();
  void on_xRot_sliderReleased();
  void on_yRot_sliderReleased();
  void on_zRot_sliderReleased();
  void on_vertex_show_clicked();

private:
  Ui::MainWindow *ui;
  QColor backgroundColor, modelColor;
  double xRotStart = 0, yRotStart = 0, zRotStart = 0;
  double xRotEnd = 0, yRotEnd = 0, zRotEnd = 0;
  bool xRotPressed = false, yRotPressed = false, zRotPressed = false;
  bool gifStart = 0, vertex = 1;
  int gifTime = 0;
  QGifImage *gifOut;
  QTimer *gifTimer;

private:
  // QWidget interface
  void rememberSettings();
  void setUpSettings();
  void _setColor(bool background, float r, float g, float b);
  void openFile(QString filename);
  void save();
  void record();

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
};
#endif // MAINWINDOW_H
