#include "MainWindow.h"

#include <QFileDialog>

#include "ui_MainWindow.h"

namespace s21 {
MainWindow::MainWindow(Scene *scene, QWidget *parent)
    : QMainWindow(parent), scene_(scene), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  ui_->openGLWidget->SetController(scene);
  scene_->AddObserver(this);

  ui_->groupBoxShader->close();
  ui_->groupBoxTransform->close();
  ui_->groupBoxMaterial->close();

  ui_->groupBoxLight->close();

  ConnectSignals();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::on_actionOpen_file_triggered() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "OBJ files (*.obj)");
  if (!filename.isEmpty()) scene_->AddModel(filename);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->isAutoRepeat()) {
    event->ignore();
    return;
  }

  if (event->key() == Qt::Key_Escape) this->close();

  scene_->KeyPressed(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat()) {
    event->ignore();
    return;
  }

  scene_->KeyReleased(event);
}

void MainWindow::OnCameraNotify() {
  static auto const &camera = scene_->GetCamera();

  ui_->dsb_CameraTransX->setValue(camera.position.x());
  ui_->dsb_CameraTransY->setValue(camera.position.y());
  ui_->dsb_CameraTransZ->setValue(camera.position.z());
  ui_->dsb_CameraPitch->setValue(camera.pitch);
  ui_->dsb_CameraYaw->setValue(camera.yaw);
}

void MainWindow::OnNotify() {
  ui_->groupBoxTransform->close();
  ui_->groupBoxLight->close();
  ui_->groupBoxMaterial->close();
  ui_->groupBoxShader->close();

  auto scene = scene_->GetScene();

  auto entities = scene->GetEntities<PickingTag>();

  for (auto entity : entities) {


    if (scene->EntityHasComponent<Transform>(entity)) {
      SetTransformUi(scene->GetComponent<Transform>(entity));
      ui_->groupBoxTransform->show();
    }

    if (scene->EntityHasComponent<Material>(entity)) {
      SetMaterialUi(scene->GetComponent<Material>(entity));
      ui_->groupBoxMaterial->show();
    }

    if (scene->EntityHasComponent<Light>(entity)) {
      SetLightUi(scene->GetComponent<Light>(entity));
      ui_->groupBoxLight->show();
    }

    if (scene->EntityHasComponent<Shader>(entity)) {
      SetShaderUi(scene->GetComponent<Shader>(entity));
      ui_->groupBoxShader->show();
    }
  }

  //  for

  //  {
  //    auto components = scene_->GetComponents<Transform>();
  //    if (components.empty()) {
  //      ui_->groupBoxTransform->close();

  //    } else {
  //      if (components.size() == 1) SetTransformUi(*components[0]);
  //      ui_->groupBoxTransform->show();
  //    }
  //  }

  //  {
  //    auto components = scene_->GetComponents<Light>();
  //    ui_->tg_LightSource->setChecked(false);

  //    if (components.empty()) {
  //      ui_->groupBoxLight->close();
  //    } else {
  //      if (components.size() == 1) {
  //        SetLightUi(*components[0]);
  //        ui_->tg_LightSource->setChecked(true);
  //      }
  //      ui_->groupBoxLight->show();
  //    }
  //  }

  //  {
  //    auto components = scene_->GetComponents<Material>();
  //    if (components.empty()) {
  //      ui_->groupBoxMaterial->close();
  //    } else {
  //      if (components.size() == 1) SetMaterialUi(*components[0]);
  //      ui_->groupBoxMaterial->show();
  //    }
  //  }

  //  {
  //    auto components = scene_->GetComponents<Shader>();
  //    if (components.empty()) {
  //      ui_->groupBoxShader->close();
  //    } else {
  //      if (components.size() == 1) SetShaderUi(*components[0]);
  //      ui_->groupBoxShader->show();
  //    }
  //  }

  //  {
  //    auto components = scene_->GetComponents<RenderTag>();
  //    if (components.empty()) {
  //      ui_->tb_Render->setChecked(false);
  //    } else if (components.size() == 1) {
  //      ui_->tb_Render->setChecked(true);
  //    }
  //  }
}

}  // namespace s21
