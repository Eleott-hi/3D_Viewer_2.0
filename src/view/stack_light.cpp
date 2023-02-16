
#include <algorithm>

#include "../others/ui_main_window.h"
#include "main_window.h"

namespace s21 {

namespace Utils {
static void BlockSignal(std::vector<QObject *> const &objects, bool block) {
  std::for_each(objects.begin(), objects.end(),
                [block](auto &object) { object->blockSignals(block); });
}
}  // namespace Utils
// =========================================================================
// ======================== OBJECT STACK SIGNALS ===========================
// =========================================================================

void MainWindow::ConnectLightSignals() {
  ui_->tg_LightSource->setAutoExclusive(false);

  // ========================== Signals to back ==========================

  // ========================== Set Panels ==========================
  connect(&signal_handler_, &SignalHandler::SetPointLightPanel, this,
          &MainWindow::SetPointLightPanel);
}
// =========================================================================
// ========================= OBJECT STACK SLOTS ============================
// =========================================================================

// ========================== Set Panels ==========================
void MainWindow::SetPointLightPanel(bool sourceLight, QVector3D const &ambient,
                                    QVector3D const &diffuse,
                                    QVector3D const &specular,
                                    QVector3D const &constants) {
  std::vector<QObject *> tmp = {
      ui_->tg_LightSource, ui_->tg_LightSource,   ui_->db_AmbientR,
      ui_->db_AmbientG,    ui_->db_AmbientB,      ui_->db_DiffuseR,
      ui_->db_DiffuseG,    ui_->db_DiffuseB,      ui_->db_SpecularR,
      ui_->db_SpecularG,   ui_->db_SpecularB,     ui_->db_ConstPoint,
      ui_->db_ConstLinear, ui_->db_ConstQuadratic};
  Utils::BlockSignal(tmp, true);

  ui_->tg_LightSource->setChecked(sourceLight);
  ui_->db_AmbientR->setValue(ambient.x());
  ui_->db_AmbientG->setValue(ambient.y());
  ui_->db_AmbientB->setValue(ambient.z());
  ui_->db_DiffuseR->setValue(diffuse.x());
  ui_->db_DiffuseG->setValue(diffuse.y());
  ui_->db_DiffuseB->setValue(diffuse.z());
  ui_->db_SpecularR->setValue(specular.x());
  ui_->db_SpecularG->setValue(specular.y());
  ui_->db_SpecularB->setValue(specular.z());
  ui_->db_ConstPoint->setValue(constants.x());
  ui_->db_ConstLinear->setValue(constants.y());
  ui_->db_ConstQuadratic->setValue(constants.z());

  Utils::BlockSignal(tmp, false);
}

// ========================== Signals to back ==========================

// ============== Light Info ==============
void MainWindow::on_tg_LightSource_toggled(bool value) {
  controller_->MakeLightSource(value);
}

void MainWindow::on_db_AmbientR_valueChanged(double value) {
  UpdatePointLightInfo(0, value);
}
void MainWindow::on_db_AmbientG_valueChanged(double value) {
  UpdatePointLightInfo(1, value);
}
void MainWindow::on_db_AmbientB_valueChanged(double value) {
  UpdatePointLightInfo(2, value);
}
void MainWindow::on_db_DiffuseR_valueChanged(double value) {
  UpdatePointLightInfo(3, value);
}
void MainWindow::on_db_DiffuseG_valueChanged(double value) {
  UpdatePointLightInfo(4, value);
}
void MainWindow::on_db_DiffuseB_valueChanged(double value) {
  UpdatePointLightInfo(5, value);
}
void MainWindow::on_db_SpecularR_valueChanged(double value) {
  UpdatePointLightInfo(6, value);
}
void MainWindow::on_db_SpecularG_valueChanged(double value) {
  UpdatePointLightInfo(7, value);
}
void MainWindow::on_db_SpecularB_valueChanged(double value) {
  UpdatePointLightInfo(8, value);
}

void MainWindow::on_db_ConstPoint_valueChanged(double value) {
  UpdatePointLightInfo(9, value);
}

void MainWindow::on_db_ConstLinear_valueChanged(double value) {
  UpdatePointLightInfo(10, value);
}

void MainWindow::on_db_ConstQuadratic_valueChanged(double value) {
  UpdatePointLightInfo(11, value);
}

void MainWindow::UpdatePointLightInfo(int index, double value) {
  std::vector<float> transform = {
      static_cast<float>(ui_->db_AmbientR->value()),
      static_cast<float>(ui_->db_AmbientG->value()),
      static_cast<float>(ui_->db_AmbientB->value()),
      static_cast<float>(ui_->db_DiffuseR->value()),
      static_cast<float>(ui_->db_DiffuseG->value()),
      static_cast<float>(ui_->db_DiffuseB->value()),
      static_cast<float>(ui_->db_SpecularR->value()),
      static_cast<float>(ui_->db_SpecularG->value()),
      static_cast<float>(ui_->db_SpecularB->value()),
      static_cast<float>(ui_->db_ConstPoint->value()),
      static_cast<float>(ui_->db_ConstLinear->value()),
      static_cast<float>(ui_->db_ConstQuadratic->value())};

  transform.at(index) = static_cast<float>(value);

  controller_->UpdatePointLightInfo(
      {transform[0], transform[1], transform[2]},   // ambient
      {transform[3], transform[4], transform[5]},   // diffuse
      {transform[6], transform[7], transform[8]},   // specular
      {transform[9], transform[10], transform[11]}  // constants
  );
}

}  // namespace s21
