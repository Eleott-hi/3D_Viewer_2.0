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
  ui_->dockShader->close();

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

void MainWindow::OnCameraNotify() {
  static auto const &camera = backend_->GetCamera();

  ui_->dsb_CameraTransX->setValue(camera.position.x());
  ui_->dsb_CameraTransY->setValue(camera.position.y());
  ui_->dsb_CameraTransZ->setValue(camera.position.z());
  ui_->dsb_CameraPitch->setValue(camera.pitch);
  ui_->dsb_CameraYaw->setValue(camera.yaw);
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
    ui_->tg_LightSource->setChecked(false);

    if (components.empty()) {
      ui_->dockLight->close();
    } else {
      if (components.size() == 1) {
        SetLightUi(*components[0]);
        ui_->tg_LightSource->setChecked(true);
      }
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

  {
    auto components = backend_->GetComponents<Shader>();
    if (components.empty()) {
      ui_->dockShader->close();
    } else {
      if (components.size() == 1) SetShaderUi(*components[0]);
      ui_->dockShader->show();
    }
  }

  {
    auto components = backend_->GetComponents<RenderTag>();
    if (components.empty()) {
      ui_->tb_Render->setChecked(false);
    } else if (components.size() == 1) {
      ui_->tb_Render->setChecked(true);
    }
  }
}

}  // namespace s21
