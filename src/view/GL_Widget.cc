#include "GL_Widget.h"

// #include <QPainter>

namespace s21 {

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  connect(timer_.get(), SIGNAL(timeout()), this, SLOT(update()));
}

void GLWidget::initializeGL() {
  controller_->Init();
  timer_->start(1000 / cadr_);

  //=================== DEBUG DELETE ========================
  std::vector<std::string> models = {
      "resources/cube.obj",
      "resources/cat.obj",
      "resources/backpack/backpack.obj",
  };
  for (auto &i : models) controller_->AddModel(i);
  //=================== DEBUG DELETE ========================
}

void GLWidget::resizeGL(int w, int h) {
  qDebug() << w << h;
  cSettings_.setWidgetSize(w, h);
  controller_->resize(w, h);
}

void GLWidget::paintGL() {
  controller_->render();
  // QString str = "Hello\nIt's me me me"; QPainter(this).drawText(QRect(1, 1,
  // 200, 100), 1, str);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    cSettings_.SetMousePosition(event->pos());
    cSettings_.MousePicking(true);
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  const QPoint &mousePos = cSettings_.GetMousePosition();
  cSettings_.Offset(mousePos - event->pos());
  cSettings_.SetMousePosition(event->pos());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {}

}  // namespace s21
