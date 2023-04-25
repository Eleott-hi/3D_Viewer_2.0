

#include <QVector>
#include <algorithm>

#include "main_window.h"
#include "ui_main_window.h"

namespace s21 {

namespace Utils {
static void BlockSignal(std::vector<QObject *> const &objects, bool block) {
  std::for_each(objects.begin(), objects.end(),
                [block](auto &object) { object->blockSignals(block); });
}
}  // namespace Utils

void MainWindow::SetPointLightPanel(bool sourceLight,  //
                                    QVector3D const &ambient,
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

void MainWindow::on_tg_LightSource_toggled(bool value) {
  controller_->MakeLightSource(value);
}

void MainWindow::UpdatePointLightInfo() {
  controller_->UpdatePointLightInfo(
      {ui_->db_AmbientR->value(),        //
       ui_->db_AmbientG->value(),        //
       ui_->db_AmbientB->value()},       // ambient
      {ui_->db_DiffuseR->value(),        //
       ui_->db_DiffuseG->value(),        //
       ui_->db_DiffuseB->value()},       // diffuse
      {ui_->db_SpecularR->value(),       //
       ui_->db_SpecularG->value(),       //
       ui_->db_SpecularB->value()},      // specular
      {ui_->db_ConstPoint->value(),      //
       ui_->db_ConstLinear->value(),     //
       ui_->db_ConstQuadratic->value()}  // constants
  );
}

void MainWindow::ConnectLightSignals() {
  ui_->tg_LightSource->setAutoExclusive(false);

  connect(&signal_handler_, &SignalHandler::SetPointLightPanel, this,
          &MainWindow::SetPointLightPanel);

  QVector<QDoubleSpinBox *> spin_boxes = {
      ui_->db_AmbientR,   ui_->db_AmbientG,    ui_->db_AmbientB,
      ui_->db_DiffuseR,   ui_->db_DiffuseG,    ui_->db_DiffuseB,
      ui_->db_SpecularR,  ui_->db_SpecularG,   ui_->db_SpecularB,
      ui_->db_ConstPoint, ui_->db_ConstLinear, ui_->db_ConstQuadratic};

  for (auto &spin_box : spin_boxes)
    connect(spin_box, &QDoubleSpinBox::valueChanged, this,
            &MainWindow::UpdatePointLightInfo);
}

}  // namespace s21
