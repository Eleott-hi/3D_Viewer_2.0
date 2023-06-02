#include "MainWindow.h"

#include <QFileDialog>
#include <map>

#include "Input.h"
#include "ui_MainWindow.h"

namespace s21 {
MainWindow::MainWindow(Scene *scene, QWidget *parent)
    : QMainWindow(parent), scene_(scene), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  ui_->openGLWidget->SetController(scene);
  scene_->AddObserver(this);

  std::map<CustomLabel *, QGroupBox *> bindings = {
      {ui_->labelGroupBoxLight, ui_->groupBoxLight},
      {ui_->labelGroupBoxShader, ui_->groupBoxShader},
      {ui_->labelGroupBoxMaterial, ui_->groupBoxMaterial},
      {ui_->labelGroupBoxTransform, ui_->groupBoxTransform},
  };

  for (auto [label, groupBox] : bindings) {
    connect(label, &CustomLabel::mousePressEvent,
            [label = label, groupBox = groupBox] {
              if (label->text().contains("▼")) {
                label->Replace("▼", "▲");
                groupBox->show();
              } else {
                label->Replace("▲", "▼");
                groupBox->close();
              }
            });

    label->Replace("▲", "▼");
    emit label->CloseSignal();
  }

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
  qDebug() << "KeyPressedEvent:" << event;

  if (event->key() == Qt::Key_Escape) qApp->closeAllWindows();

  if (event->key() == Qt::Key_Space) {
    auto scene = scene_->GetScene();

    if (scene->EntityHasComponent<PickingTag>(85))
      scene->RemoveComponent<PickingTag>(85);
    else
      scene->AddComponent<PickingTag>(85);
  }

  Input::KeyPressed((Qt::Key)event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat()) {
    event->ignore();
    return;
  }

  qDebug() << "KeyReleasedEvent:" << event;
  Input::KeyReleased((Qt::Key)event->key());
}

void MainWindow::OnCameraNotify() {
  auto scene = scene_->GetScene();
  auto const &camera = scene->GetComponent<Camera>(Utils::GetCamera(scene));

  SetCameraUi(camera);
}

void MainWindow::OnNotify() {
  // ui_->labelGroupBoxLight->Replace("▲", "▼");
  // ui_->labelGroupBoxShader->Replace("▲", "▼");
  // ui_->labelGroupBoxMaterial->Replace("▲", "▼");
  // ui_->labelGroupBoxTransform->Replace("▲", "▼");
  // emit ui_->labelGroupBoxLight->CloseSignal();
  // emit ui_->labelGroupBoxShader->CloseSignal();
  // emit ui_->labelGroupBoxMaterial->CloseSignal();
  // emit ui_->labelGroupBoxTransform->CloseSignal();

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

  if (entities.empty()) {
    emit ui_->labelGroupBoxLight->CloseSignal();
    emit ui_->labelGroupBoxShader->CloseSignal();
    emit ui_->labelGroupBoxMaterial->CloseSignal();
    emit ui_->labelGroupBoxTransform->CloseSignal();
  }
}

}  // namespace s21
