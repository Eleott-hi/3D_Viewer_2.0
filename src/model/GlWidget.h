#ifndef SRC_HEADERS_GLWIDGET_H
#define SRC_HEADERS_GLWIDGET_H

#include <QMouseEvent>
#include <QOpenGLWidget>
// #include <QPainter>
#include <QTimer>

#include "controller.h"

namespace s21 {
class GLWidget : public QOpenGLWidget {
 public:
  GLWidget(QWidget *parent = nullptr);

  ~GLWidget() = default;
  void SetController(Controller *c) { controller_ = c; }

 protected:
  //=================== QOpenGLWidget interface ========================
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  //====================== QWidget interface ===========================
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event) override;

 private:
  float cadr_ = 100;
  Controller *controller_ = nullptr;
  CommonSettings &cSettings_ = CommonSettings::GetInstance();
  std::shared_ptr<QTimer> timer_ = std::make_shared<QTimer>();
};

}  // namespace s21

#endif  // SRC_HEADERS_GLWIDGET_H
