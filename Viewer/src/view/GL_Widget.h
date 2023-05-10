#ifndef SRC_HEADERS_GLWIDGET_H
#define SRC_HEADERS_GLWIDGET_H

#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QTimer>

#include "Backend.h"

namespace s21 {
class GLWidget : public QOpenGLWidget {
 public:
  GLWidget(QWidget *parent = nullptr);

  ~GLWidget() = default;
  void SetController(Backend *backend) { backend_ = backend; }

 protected:
  void paintGL() override;
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

 private:
  QTimer timer_;
  Backend *backend_ = nullptr;
};

}  // namespace s21

#endif  // SRC_HEADERS_GLWIDGET_H
