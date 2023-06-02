#include "GL_Widget.h"

#include "Input.h"
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
  // scene_->AddModel(dir + "objects/cat.obj");
}

void GLWidget::resizeGL(int w, int h) { scene_->WindowResize(w, h); }

void GLWidget::paintGL() {
  Input::MousePosition(mapFromGlobal(QCursor::pos()));

  scene_->Render();

  Input::MouseLastPosition(Input::MousePosition());
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  qDebug() << "MousePressedEvent:" << event;
  this->setFocus();
  Input::MousePressed(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  qDebug() << "MouseReleasedEvent:" << event;
  Input::MouseReleased(event->button());
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  qDebug() << "MouseDoubleClicked:" << event;
  Input::MouseDoubleClick(true);
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  qDebug() << "MouseScrolledEvent:" << event;

  auto scene = scene_->GetScene();
  auto &camera = scene->GetComponent<Camera>(Utils::GetCamera(scene));

  float scroll = event->angleDelta().y() / 80.0;
  camera.zoom += scroll;
}

}  // namespace s21
