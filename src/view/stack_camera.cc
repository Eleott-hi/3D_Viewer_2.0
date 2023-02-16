#include <QColorDialog>
#include <QFileDialog>

#include "../others/ui_main_window.h"
#include "main_window.h"

namespace s21 {

namespace Utils {
static void BlockSignal(std::vector<QObject *> const &objects, bool block) {
  std::for_each(objects.begin(), objects.end(),
                [block](auto &object) { object->blockSignals(block); });
}

static QColor getLabelColor(QLabel *label) {
  QString style = label->styleSheet();
  return QColor("#" + style.section("#", -1));
}

} // namespace Utils

// ======================== CAMERA STACK SIGNALS ===========================
void MainWindow::ConnectCameraSignals() {
  connect(&signal_handler_, &SignalHandler::SetStackCamera, this,
          &MainWindow::SetStackCamera);
  connect(&signal_handler_, &SignalHandler::SetCameraPanel, this,
          &MainWindow::SetCameraPanel);
  connect(&signal_handler_, &SignalHandler::SetObjectsCount,
          ui_->ObjectsCountLabel, &QLabel::setText);
  //  connect(ui_->btn_perspective, &QRadioButton::toggled, &cSettings_,
  //          &CommonSettings::Perspective);

  ui_->btn_perspective->setAutoExclusive(false);
}

// ========================= CAMERA STACK SLOTS ============================
void MainWindow::SetStackCamera() { ui_->stackedWidget->setCurrentIndex(0); }

void MainWindow::SetCameraPanel(QVector3D position, QVector3D rotation,
                                float zoom) {
  std::vector<QObject *> tmp = {ui_->xTrans, ui_->yTrans, ui_->zTrans,
                                ui_->xRot,   ui_->yRot,   ui_->CameraZoom};

  Utils::BlockSignal(tmp, true);

  ui_->xTrans->setValue((int)position.x());
  ui_->yTrans->setValue((int)position.y());
  ui_->zTrans->setValue((int)position.z());
  ui_->xRot->setValue((int)rotation.x());
  ui_->yRot->setValue((int)rotation.y());
  ui_->CameraZoom->setValue((int)zoom);

  Utils::BlockSignal(tmp, false);
}

void MainWindow::on_btn_BackColor_clicked() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (color.isValid()) {
    ui_->l_BackColor->setStyleSheet("background-color: #" +
                                    QString::number(color.rgb(), 16));

    cSettings_.SetBackColor(color);
  }
}

void MainWindow::on_btn_AddObject_clicked() {
  //   QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
  //   "OBJ files (*.obj)");   if (!filename.isEmpty()) {
  //   controller_->addModel(filename.toStdString());}

  std::vector<std::string> models = {"resources/baseball_cap_1082k.obj"};
  for (auto &i : models)
    controller_->AddModel(i);
}

void MainWindow::on_btn_perspective_toggled(bool value) {
  controller_->SetPerspective(value);
}

void MainWindow::on_xTrans_valueChanged(int value) {
  UpdateCameraInfo(0, value);
}
void MainWindow::on_yTrans_valueChanged(int value) {
  UpdateCameraInfo(1, value);
}
void MainWindow::on_zTrans_valueChanged(int value) {
  UpdateCameraInfo(2, value);
}
void MainWindow::on_xRot_valueChanged(int value) { UpdateCameraInfo(3, value); }
void MainWindow::on_yRot_valueChanged(int value) { UpdateCameraInfo(4, value); }

void MainWindow::on_CameraZoom_valueChanged(int value) {
  UpdateCameraInfo(6, value);
}

void MainWindow::UpdateCameraInfo(int index, int value) {
  std::vector<float> cameraInfo = {
      static_cast<float>(ui_->xTrans->value()),
      static_cast<float>(ui_->yTrans->value()),
      static_cast<float>(ui_->zTrans->value()),
      static_cast<float>(ui_->xRot->value()),
      static_cast<float>(ui_->yRot->value()),
      0,
      static_cast<float>(ui_->CameraZoom->value())};

  cameraInfo.at(index) = static_cast<float>(value);

  qDebug() << cameraInfo;

  controller_->UpdateCameraInfo(
      {cameraInfo[0], cameraInfo[1], cameraInfo[2]}, // translation
      {cameraInfo[3], cameraInfo[4], cameraInfo[5]}, // rotation
      cameraInfo[6]                                  // zoom
  );
}

void MainWindow::on_btn_ResetCamera_clicked() {
  SetCameraPanel({0, 0, 0}, {0, -90, 0}, 1);

  controller_->UpdateCameraInfo({0, 0, 0}, // translation
                                {0, 0, 0}, // rotation
                                1          // zoom
  );
}

} // namespace s21
