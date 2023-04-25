#include "main_window.h"

#include "ui_main_window.h"

namespace s21 {
MainWindow::MainWindow(Controller *c, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(c) {
  ui_->setupUi(this);
  ui_->openGLWidget->SetController(c);

  ConnectSignals();
  ConnectCameraSignals();
  ConnectObjectSignals();
  ConnectLightSignals();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::ConnectSignals() {
  connect(ui_->actionQuit, &QAction::triggered, [&] { qApp->quit(); });
}

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

void MainWindow::on_actionOpen_file_triggered() {
  //   QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
  //   "OBJ files (*.obj)");   if (!filename.isEmpty()) {
  //   controller_->addModel(filename.toStdString());}

  std::vector<std::string> models = {"resources/baseball_cap_1082k.obj"};
  for (auto &i : models) controller_->AddModel(i);
}

}  // namespace s21