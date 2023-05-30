#ifndef SRC_HEADERS_GLWIDGET_H
#define SRC_HEADERS_GLWIDGET_H

#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QTimer>

#include "Scene.h"

namespace s21 {
class GLWidget : public QOpenGLWidget {
 public:
  GLWidget(QWidget *parent = nullptr);

  ~GLWidget() = default;
  void SetController(Scene *scene) { scene_ = scene; }

 protected:
  void paintGL() override;
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

 private:
  QTimer timer_;
  Scene *scene_ = nullptr;
};

}  // namespace s21

#endif  // SRC_HEADERS_GLWIDGET_H
