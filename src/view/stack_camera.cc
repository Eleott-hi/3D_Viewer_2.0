#include <QColorDialog>
#include <QFileDialog>
#include <QVector>

#include "main_window.h"
#include "ui_main_window.h"

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

void MainWindow::SetStackCamera() { ui_->stackedWidget->setCurrentIndex(0); }

void MainWindow::SetCameraPanel(QVector3D position, QVector3D rotation,
                                float zoom) {
  static std::vector<QObject *> tmp = {ui_->xTrans, ui_->yTrans,
                                       ui_->zTrans, ui_->xRot,
                                       ui_->yRot,   ui_->CameraZoom};

  Utils::BlockSignal(tmp, true);

  ui_->xTrans->setValue(position.x());
  ui_->yTrans->setValue(position.y());
  ui_->zTrans->setValue(position.z());
  ui_->xRot->setValue(rotation.x());
  ui_->yRot->setValue(rotation.y());
  ui_->CameraZoom->setValue(zoom);

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

void MainWindow::UpdateCameraInfo() {

  controller_->UpdateCameraInfo(QVector3D(ui_->xTrans->value(),  //
                                          ui_->yTrans->value(),  //
                                          ui_->zTrans->value()), // translation
                                QVector3D(ui_->xRot->value(),    //
                                          ui_->yRot->value(),    //
                                          0),                    // rotation
                                ui_->CameraZoom->value()         // zoom
  );
}

void MainWindow::on_btn_ResetCamera_clicked() {
  SetCameraPanel({0, 0, 0}, {0, -90, 0}, 1);

  controller_->UpdateCameraInfo({0, 0, 0}, // translation
                                {0, 0, 0}, // rotation
                                1          // zoom
  );
}

void MainWindow::ConnectCameraSignals() {
  connect(&signal_handler_, &SignalHandler::SetStackCamera, this,
          &MainWindow::SetStackCamera);
  connect(&signal_handler_, &SignalHandler::SetCameraPanel, this,
          &MainWindow::SetCameraPanel);
  connect(&signal_handler_, &SignalHandler::SetObjectsCount,
          ui_->ObjectsCountLabel, &QLabel::setText);
  //  connect(ui_->btn_perspective, &QRadioButton::toggled, &cSettings_,
  //          &CommonSettings::Perspective);

  connect(ui_->btn_perspective, &QRadioButton::toggled, controller_,
          &Controller::SetPerspective);

  ui_->btn_perspective->setAutoExclusive(false);

  QVector<QSlider *> sliders = {ui_->xTrans, ui_->yTrans, ui_->zTrans,
                                ui_->xRot,   ui_->yRot,   ui_->CameraZoom};

  for (auto &slider : sliders)
    connect(slider, &QSlider::valueChanged, this,
            &MainWindow::UpdateCameraInfo);
}

} // namespace s21
