#include "MainWindow.h"

#include <QFileDialog>

#include "ui_MainWindow.h"

namespace s21 {
MainWindow::MainWindow(Backend *backend, QWidget *parent)
    : QMainWindow(parent), backend_(backend), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  ui_->openGLWidget->SetController(backend);
  backend_->AddObserver(this);

  ui_->dockTransformation->close();
  ui_->dockMaterial->close();
  ui_->dockLight->close();

  ConnectSignals();
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

void MainWindow::OnNotify() {
  {
    auto components = backend_->GetComponents<Transform>();
    if (components.empty()) {
      ui_->dockTransformation->close();
    } else {
      if (components.size() == 1) SetTransformUi(*components[0]);
      ui_->dockTransformation->show();
    }
  }

  {
    auto components = backend_->GetComponents<Light>();
    if (components.empty()) {
      ui_->dockLight->close();
    } else {
      if (components.size() == 1) SetLightUi(*components[0]);
      ui_->dockLight->show();
    }
  }

  {
    auto components = backend_->GetComponents<Material>();
    if (components.empty()) {
      ui_->dockMaterial->close();
    } else {
      if (components.size() == 1) SetMaterialUi(*components[0]);
      ui_->dockMaterial->show();
    }
  }

  //  for (auto &component : components) {
  //    qDebug() << component->translation;
  //    qDebug() << component->rotation;
  //  }
}

}  // namespace s21
