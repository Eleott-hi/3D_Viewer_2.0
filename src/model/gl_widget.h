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
  GLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {
    connect(timer_.get(), SIGNAL(timeout()), this, SLOT(update()));
  }

  ~GLWidget() = default;

  void SetController(Controller *c) {  //
    controller_ = c;
  }

 protected:
  //=================== QOpenGLWidget interface ========================
  void initializeGL() override {
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

  void resizeGL(int w, int h) override {
    qDebug() << w << h;
    cSettings_.setWidgetSize(w, h);
    controller_->resize(w, h);
  }

  void paintGL() override {
    controller_->render();
    // QString str = "Hello\nIt's me me me"; QPainter(this).drawText(QRect(1, 1,
    // 200, 100), 1, str);
  }

  //====================== QWidget interface ===========================
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      cSettings_.SetMousePosition(event->pos());
      cSettings_.MousePicking(true);
    }
  }

  void mouseMoveEvent(QMouseEvent *event) override {
    const QPoint &mousePos = cSettings_.GetMousePosition();
    cSettings_.Offset(mousePos - event->pos());
    cSettings_.SetMousePosition(event->pos());
  }

  void mouseReleaseEvent(QMouseEvent *event) override {}

 private:
  float cadr_ = 100;
  Controller *controller_ = nullptr;
  CommonSettings &cSettings_ = CommonSettings::GetInstance();
  std::shared_ptr<QTimer> timer_ = std::make_shared<QTimer>();
};

}  // namespace s21

#endif  // SRC_HEADERS_GLWIDGET_H
