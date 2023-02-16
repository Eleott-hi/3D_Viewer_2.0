#include <QColorDialog>
#include <QFileDialog>
#include <algorithm>

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

static void setLabelColor(QLabel *label, QColor color) {
  label->setStyleSheet("background-color: #" +
                       QString::number(color.rgb(), 16));
}

} // namespace Utils

// ======================== OBJECT STACK SIGNALS ===========================
void MainWindow::ConnectObjectSignals() {
  ui_->toggle_AxisLines->setAutoExclusive(false);
  ui_->SmoothPoints_2->setAutoExclusive(false);
  ui_->DashedLines_2->setAutoExclusive(false);
  ui_->ShowVertex->setAutoExclusive(false);
  ui_->showLines->setAutoExclusive(false);
  // ========================== Signals to back ==========================
  connect(&signal_handler_, &SignalHandler::SetStackObject, this,
          &MainWindow::SetStackObject);
  connect(ui_->toggle_AxisLines, &QRadioButton::toggled, controller_,
          &Controller::UpdateAxisInfo);

  // ========================== Set Panels ==========================
  connect(&signal_handler_, &SignalHandler::SetInfoPanel, this,
          &MainWindow::SetInfoPanel);
  connect(&signal_handler_, &SignalHandler::SetTransformPanel, this,
          &MainWindow::SetTransformPanel);
  connect(&signal_handler_, &SignalHandler::SetPointPanel, this,
          &MainWindow::SetPointPanel);
  connect(&signal_handler_, &SignalHandler::SetLinePanel, this,
          &MainWindow::SetLinePanel);
  connect(&signal_handler_, &SignalHandler::SetTexturePanel, this,
          &MainWindow::SetTexturePanel);
  connect(&signal_handler_, &SignalHandler::SetMaterialPanel, this,
          &MainWindow::SetMaterialPanel);
  connect(&signal_handler_, &SignalHandler::SetAxisPanel, this,
          &MainWindow::SetAxisPanel);
}

// ========================= OBJECT STACK SLOTS ============================
void MainWindow::SetStackObject() { ui_->stackedWidget->setCurrentIndex(1); }

// ========================== Set Panels ==========================
void MainWindow::SetInfoPanel(QString const &file, uint32_t vertices,
                              uint32_t indices) {
  ui_->lb_ObjectFilename->setText(file.section("/", -1));
  ui_->lb_Vertices->setText(QString::number(vertices));
  ui_->lb_Facets->setText(QString::number(indices));
}

void MainWindow::SetTransformPanel(QVector3D position, QVector3D rotation,
                                   float scale) {
  std::vector<QObject *> tmp = {ui_->xTrans_2, ui_->yTrans_2, ui_->zTrans_2,
                                ui_->xRot_2,   ui_->yRot_2,   ui_->zRot_2,
                                ui_->Scale_2};

  Utils::BlockSignal(tmp, true);

  ui_->xTrans_2->setValue((int)position.x());
  ui_->yTrans_2->setValue((int)position.y());
  ui_->zTrans_2->setValue((int)position.z());
  ui_->xRot_2->setValue((int)rotation.x());
  ui_->yRot_2->setValue((int)rotation.y());
  ui_->zRot_2->setValue((int)rotation.z());
  ui_->Scale_2->setValue((int)scale);

  Utils::BlockSignal(tmp, false);
}

void MainWindow::SetPointPanel(bool show, bool smooth, int size, QColor color) {
  std::vector<QObject *> tmp = {ui_->ShowVertex, ui_->SmoothPoints_2,
                                ui_->PointSize_2};
  Utils::BlockSignal(tmp, true);

  ui_->ShowVertex->setChecked(show);
  ui_->SmoothPoints_2->setChecked(smooth);
  ui_->PointSize_2->setValue(size);
  Utils::setLabelColor(ui_->l_PointColor, color);

  Utils::BlockSignal(tmp, false);
}

void MainWindow::SetLinePanel(bool show, bool dashed, int size, QColor color) {
  std::vector<QObject *> tmp = {ui_->showLines, ui_->DashedLines_2,
                                ui_->LineWidth_2};
  Utils::BlockSignal(tmp, true);

  ui_->showLines->setChecked(show);
  ui_->DashedLines_2->setChecked(dashed);
  ui_->LineWidth_2->setValue(size);
  Utils::setLabelColor(ui_->l_ObjectColor, color);

  Utils::BlockSignal(tmp, false);
}

void MainWindow::SetTexturePanel(int index) {
  std::vector<QObject *> tmp = {ui_->cb_RenderTechnique};
  Utils::BlockSignal(tmp, true);

  ui_->cb_RenderTechnique->setCurrentIndex(index);

  Utils::BlockSignal(tmp, false);
}

void MainWindow::SetMaterialPanel(float shininess) {

  std::vector<QObject *> tmp = {ui_->Shininess};
  Utils::BlockSignal(tmp, true);

  ui_->Shininess->setValue((int)shininess);

  Utils::BlockSignal(tmp, false);
}

void MainWindow::SetAxisPanel(bool value) {
  std::vector<QObject *> tmp = {ui_->toggle_AxisLines};
  Utils::BlockSignal(tmp, true);

  ui_->toggle_AxisLines->setChecked(value);

  Utils::BlockSignal(tmp, false);
}

// ========================== Signals to back ==========================

// ============== Transform Info ==============
void MainWindow::on_xTrans_2_valueChanged(int value) {
  UpdateTransformInfo(0, value);
}

void MainWindow::on_yTrans_2_valueChanged(int value) {
  UpdateTransformInfo(1, value);
}

void MainWindow::on_zTrans_2_valueChanged(int value) {
  UpdateTransformInfo(2, value);
}

void MainWindow::on_xRot_2_valueChanged(int value) {
  UpdateTransformInfo(3, value);
}

void MainWindow::on_yRot_2_valueChanged(int value) {
  UpdateTransformInfo(4, value);
}

void MainWindow::on_zRot_2_valueChanged(int value) {
  UpdateTransformInfo(5, value);
}

void MainWindow::on_Scale_2_valueChanged(int value) {
  UpdateTransformInfo(6, value);
}

void MainWindow::UpdateTransformInfo(int index, int value) {
  std::vector<float> transform = {static_cast<float>(ui_->xTrans_2->value()),
                                  static_cast<float>(ui_->yTrans_2->value()),
                                  static_cast<float>(ui_->zTrans_2->value()),
                                  static_cast<float>(ui_->xRot_2->value()),
                                  static_cast<float>(ui_->yRot_2->value()),
                                  static_cast<float>(ui_->zRot_2->value()),
                                  static_cast<float>(ui_->Scale_2->value())};

  transform.at(index) = static_cast<float>(value);

  controller_->UpdateTransformInfo(
      {transform[0], transform[1], transform[2]}, // translation
      {transform[3], transform[4], transform[5]}, // rotation
      transform[6]                                // scale
  );
}

// ============== Point Info ==============

void MainWindow::on_ShowVertex_toggled(bool value) {
  controller_->UpdatePointInfo(value, ui_->SmoothPoints_2->isChecked(),
                               ui_->PointSize_2->value(),
                               Utils::getLabelColor(ui_->l_PointColor));
}

void MainWindow::on_SmoothPoints_2_toggled(bool value) {
  controller_->UpdatePointInfo(ui_->ShowVertex->isChecked(), value,
                               ui_->PointSize_2->value(),
                               Utils::getLabelColor(ui_->l_PointColor));
}

void MainWindow::on_PointSize_2_valueChanged(int value) {
  controller_->UpdatePointInfo(ui_->ShowVertex->isChecked(),
                               ui_->SmoothPoints_2->isChecked(), value,
                               Utils::getLabelColor(ui_->l_PointColor));
}

void MainWindow::on_DotsColor_2_clicked() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (color.isValid()) {
    Utils::setLabelColor(ui_->l_PointColor, color);
    controller_->UpdatePointInfo(ui_->ShowVertex->isChecked(),
                                 ui_->SmoothPoints_2->isChecked(),
                                 ui_->PointSize_2->value(), color);
  }
}

// ============== Line Info ==============
void MainWindow::on_showLines_toggled(bool value) {
  controller_->UpdateLineInfo(value, ui_->DashedLines_2->isChecked(),
                              ui_->LineWidth_2->value(),
                              Utils::getLabelColor(ui_->l_ObjectColor));
}

void MainWindow::on_DashedLines_2_toggled(bool value) {
  controller_->UpdateLineInfo(ui_->showLines->isChecked(), value,
                              ui_->LineWidth_2->value(),
                              Utils::getLabelColor(ui_->l_ObjectColor));
}

void MainWindow::on_LineWidth_2_valueChanged(int value) {
  controller_->UpdateLineInfo(ui_->showLines->isChecked(),
                              ui_->DashedLines_2->isChecked(), value,
                              Utils::getLabelColor(ui_->l_ObjectColor));
}

void MainWindow::on_LinesColor_2_clicked() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (color.isValid()) {
    Utils::setLabelColor(ui_->l_ObjectColor, color);
    controller_->UpdateLineInfo(ui_->showLines->isChecked(),
                                ui_->DashedLines_2->isChecked(),
                                ui_->LineWidth_2->value(), color);
  }
}

// ================= Texture Technique ===================

void MainWindow::on_cb_RenderTechnique_currentIndexChanged(int index) {
  controller_->UpdateTextureTechnique(index);
}

void MainWindow::on_btn_LoadTexture_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File");
  if (!filename.isEmpty()) {
    controller_->LoadTexture(filename.toStdString());
  }
}

// ============== Reset ==============
void MainWindow::on_btn_ResetObject_clicked() {
  SetTransformPanel({0, 0, 0}, {0, 0, 0}, 1);

  controller_->UpdateTransformInfo({0, 0, 0}, // translation
                                   {0, 0, 0}, // rotation
                                   1          // scale
  );
}

void MainWindow::on_Shininess_valueChanged(int value) {
  controller_->UpdateMaterialInfo(static_cast<float>(value));
}


void MainWindow::on_btn_DeleteObject_clicked()
{
    controller_->DeleteObject();
}

} // namespace s21
