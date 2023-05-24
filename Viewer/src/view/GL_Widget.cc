#include "GL_Widget.h"

#include "MainWindow.h"

namespace s21 {

QString dir =  //
    // "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/";
"C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Tutorials/resources/";

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  connect(&timer_, &QTimer::timeout, [&] { update(); });
}

void GLWidget::initializeGL() {
  scene_->Init(this);

  int fps = 1000;
  timer_.start(1000 / fps);

  //  for (size_t i = 0; i < 10; i++)
  scene_->AddModel(dir + "objects/backpack/backpack.obj");
  scene_->AddModel(dir + "objects/cat.obj");
}

void GLWidget::resizeGL(int w, int h) { scene_->WindowResize(w, h); }

void GLWidget::paintGL() { scene_->Render(); }

void GLWidget::mousePressEvent(QMouseEvent *event) {
  this->setFocus();
  if (event->button() == Qt::LeftButton) scene_->MousePressed(event->pos());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  scene_->MouseMoved(event->pos());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) scene_->MouseReleased(event->pos());
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  scene_->MouseDoubleClicked(event->pos());
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  auto numDegrees = event->angleDelta();
  scene_->MouseScrolled(numDegrees.y() / 80.0);
}

}  // namespace s21
