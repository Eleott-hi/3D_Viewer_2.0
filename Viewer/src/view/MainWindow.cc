#include "MainWindow.h"

#include <QFileDialog>

#include "ui_MainWindow.h"

namespace s21 {
MainWindow::MainWindow(Scene *scene, QWidget *parent)
    : QMainWindow(parent), scene_(scene), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  ui_->openGLWidget->SetController(scene);
  scene_->AddObserver(this);

  emit ui_->labelGroupBoxLight->CloseSignal();
  emit ui_->labelGroupBoxShader->CloseSignal();
  emit ui_->labelGroupBoxMaterial->CloseSignal();
  emit ui_->labelGroupBoxTransform->CloseSignal();

  connect(ui_->labelCameraTransform, &CustomLabel::mousePressEvent, [this] {
    if (ui_->labelCameraTransform->text().contains("▼")) {
      ui_->labelCameraTransform->Replace("▼", "▲");
      ui_->groupBoxCameraTransform->show();
    } else {
      ui_->labelCameraTransform->Replace("▲", "▼");
      ui_->groupBoxCameraTransform->close();
    }
  });

  connect(ui_->labelCameraPlanes, &CustomLabel::mousePressEvent, [this] {
    if (ui_->labelCameraPlanes->text().contains("▼")) {
      ui_->labelCameraPlanes->Replace("▼", "▲");
      ui_->groupBoxCameraPlanes->show();
    } else {
      ui_->labelCameraPlanes->Replace("▲", "▼");
      ui_->groupBoxCameraPlanes->close();
    }
  });

  connect(ui_->labelGroupBoxTransform, &CustomLabel::mousePressEvent, [this] {
    if (ui_->labelGroupBoxTransform->text().contains("▼")) {
      ui_->labelGroupBoxTransform->Replace("▼", "▲");
      ui_->groupBoxTransform->show();
    } else {
      ui_->labelGroupBoxTransform->Replace("▲", "▼");
      ui_->groupBoxTransform->close();
    }
  });

  connect(ui_->labelGroupBoxLight, &CustomLabel::mousePressEvent, [this] {
    if (ui_->labelGroupBoxLight->text().contains("▼")) {
      ui_->labelGroupBoxLight->Replace("▼", "▲");
      ui_->groupBoxLight->show();
    } else {
      ui_->labelGroupBoxLight->Replace("▲", "▼");
      ui_->groupBoxLight->close();
    }
  });

  connect(ui_->labelGroupBoxMaterial, &CustomLabel::mousePressEvent, [this] {
    if (ui_->labelGroupBoxMaterial->text().contains("▼")) {
      ui_->labelGroupBoxMaterial->Replace("▼", "▲");
      ui_->groupBoxMaterial->show();
    } else {
      ui_->labelGroupBoxMaterial->Replace("▲", "▼");
      ui_->groupBoxMaterial->close();
    }
  });

  connect(ui_->labelGroupBoxShader, &CustomLabel::mousePressEvent, [this] {
    if (ui_->labelGroupBoxShader->text().contains("▼")) {
      ui_->labelGroupBoxShader->Replace("▼", "▲");
      ui_->groupBoxShader->show();
    } else {
      ui_->labelGroupBoxShader->Replace("▲", "▼");
      ui_->groupBoxShader->close();
    }
  });

  // Q_ASSERT(false);

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
  auto scene = scene_->GetScene();
  auto const &camera = scene->GetComponent<Camera>(Utils::GetCamera(scene));

  SetCameraUi(camera);
}

void MainWindow::OnNotify() {
  ui_->labelGroupBoxLight->Replace("▲", "▼");
  ui_->labelGroupBoxShader->Replace("▲", "▼");
  ui_->labelGroupBoxMaterial->Replace("▲", "▼");
  ui_->labelGroupBoxTransform->Replace("▲", "▼");
  emit ui_->labelGroupBoxLight->CloseSignal();
  emit ui_->labelGroupBoxShader->CloseSignal();
  emit ui_->labelGroupBoxMaterial->CloseSignal();
  emit ui_->labelGroupBoxTransform->CloseSignal();

  auto scene = scene_->GetScene();
  auto entities = scene->GetEntities<PickingTag>();

  for (auto entity : entities) {
    if (scene->EntityHasComponent<Transform>(entity)) {
      SetTransformUi(scene->GetComponent<Transform>(entity));

      emit ui_->labelGroupBoxTransform->ShowSignal();
    }

    if (scene->EntityHasComponent<Material>(entity)) {
      SetMaterialUi(scene->GetComponent<Material>(entity));

      emit ui_->labelGroupBoxMaterial->ShowSignal();
    }

    if (scene->EntityHasComponent<Light>(entity)) {
      SetLightUi(scene->GetComponent<Light>(entity));

      emit ui_->labelGroupBoxLight->ShowSignal();
    }

    if (scene->EntityHasComponent<Shader>(entity)) {
      SetShaderUi(scene->GetComponent<Shader>(entity));

      emit ui_->labelGroupBoxShader->ShowSignal();
    }
  }
}

}  // namespace s21
