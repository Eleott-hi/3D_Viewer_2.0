#include "MainWindow.h"

#include <QFileDialog>

#include "ui_MainWindow.h"

namespace s21 {
MainWindow::MainWindow(Backend *backend, QWidget *parent)
    : QMainWindow(parent), backend_(backend), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  ui_->openGLWidget->SetController(backend);
  this->grabKeyboard();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::on_actionOpen_file_triggered() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "OBJ files (*.obj)");
  if (!filename.isEmpty()) backend_->AddModel(filename);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->isAutoRepeat()) {
    event->ignore();
    return;
  }

  if (event->key() == Qt::Key_Escape) this->close();

  backend_->KeyPressed(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat()) {
    event->ignore();
    return;
  }

  backend_->KeyReleased(event);
}

}  // namespace s21
