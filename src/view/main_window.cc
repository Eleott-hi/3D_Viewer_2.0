#include "main_window.h"

#include "../others/ui_main_window.h"

namespace s21 {
MainWindow::MainWindow(Controller *c, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(c) {
  ui_->setupUi(this);
  ui_->openGLWidget->SetController(c);

  ConnectCameraSignals();
  ConnectObjectSignals();
  ConnectLightSignals();
}

MainWindow::~MainWindow() { delete ui_; }

// =============================== KEYBORD ===============================
void MainWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      this->close();
      break;
    case Qt::Key_W:
      cSettings_.MoveCamera(CameraDirection::FORWARD);
      break;
    case Qt::Key_S:
      cSettings_.MoveCamera(CameraDirection::BACKWARD);
      break;
    case Qt::Key_D:
      cSettings_.MoveCamera(CameraDirection::RIGHT);
      break;
    case Qt::Key_A:
      cSettings_.MoveCamera(CameraDirection::LEFT);
      break;

    default:
      break;
  }
}

}  // namespace s21
