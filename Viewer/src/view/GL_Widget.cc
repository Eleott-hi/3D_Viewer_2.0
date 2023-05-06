#include "GL_Widget.h"

// #include <QPainter>

namespace s21 {

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
}

void GLWidget::initializeGL() {
  backend_->Init(this);

  int fps = 100;
  timer_.start(1000 / fps);

  backend_->AddModel(
      "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Viewer/src/others/"
      "resources/backpack/backpack.obj"
      // "/opt/goinfre/pintoved/3D_Viewer_2.0/Viewer/src/others/resources/"
      // "cube.obj"
  );

  // backend_->LoadTexture(
  //     "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Viewer/src/others/"
  //     "resources/backpack/diffuse.jpg"  //
  //     // "/opt/goinfre/pintoved/3D_Viewer_2.0/Viewer/src/others/resources/"
  //     // "awesomeface.png"
  // );
}

void GLWidget::resizeGL(int w, int h) { backend_->WindowResize(w, h); }

void GLWidget::paintGL() { backend_->Render(); }

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) last_pos_ = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  // if (event->button() == Qt::LeftButton) {
  backend_->MouseMoved(last_pos_ - event->pos());
  last_pos_ = event->pos();
  // }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  backend_->MouseDoubleClicked(event->pos());
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  auto numDegrees = event->angleDelta();
  backend_->MouseScrolled(numDegrees.y() / 80.0);
}

}  // namespace s21
