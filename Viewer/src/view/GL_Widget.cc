#include "GL_Widget.h"

// #include <QPainter>
// #include <QCoreApplication>
#include "MainWindow.h"

namespace s21 {

QString dir =  //
               // "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/";
    "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Tutorials/resources/";

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  connect(&timer_, &QTimer::timeout, [&] { update(); });
}

void GLWidget::initializeGL() {
  backend_->Init(this);

  int fps = 100;
  timer_.start(1000 / fps);

  //  for (size_t i = 0; i < 10; i++)
  backend_->AddModel(dir + "backpack/backpack.obj");
  backend_->AddModel(dir + "cat.obj");
}

void GLWidget::resizeGL(int w, int h) { backend_->WindowResize(w, h); }

void GLWidget::paintGL() { backend_->Render(); }

void GLWidget::mousePressEvent(QMouseEvent *event) {
  this->setFocus();
  if (event->button() == Qt::LeftButton) backend_->MousePressed(event->pos());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  backend_->MouseMoved(event->pos());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) backend_->MouseReleased(event->pos());
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  backend_->MouseDoubleClicked(event->pos());
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  auto numDegrees = event->angleDelta();
  backend_->MouseScrolled(numDegrees.y() / 80.0);
}

}  // namespace s21
