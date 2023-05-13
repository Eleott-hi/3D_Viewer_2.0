#include <QFileDialog>

#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace s21 {

void MainWindow::ConnectSignals() {
  ConnectTransformUi();
  ConnectLightUi();
  ConnectMaterialUi();
}

void MainWindow::SetLightUi(Light const &component) {
  ui_->lightPositionX->setValue(component.position.x());
  ui_->lightPositionY->setValue(component.position.y());
  ui_->lightPositionZ->setValue(component.position.z());
  ui_->lightDirectionX->setValue(component.direction.x());
  ui_->lightDirectionY->setValue(component.direction.y());
  ui_->lightDirectionZ->setValue(component.direction.z());
  ui_->lightAmbientR->setValue(component.ambient.x());
  ui_->lightAmbientG->setValue(component.ambient.y());
  ui_->lightAmbientB->setValue(component.ambient.z());
  ui_->lightDiffuseR->setValue(component.diffuse.x());
  ui_->lightDiffuseG->setValue(component.diffuse.y());
  ui_->lightDiffuseB->setValue(component.diffuse.z());
  ui_->lightSpecularR->setValue(component.specular.x());
  ui_->lightSpecularG->setValue(component.specular.y());
  ui_->lightSpecularB->setValue(component.specular.z());
  ui_->lightInnerCone->setValue(component.inner_cone);
  ui_->lightOuterCone->setValue(component.outer_cone);
}

void MainWindow::SetMaterialUi(Material const &component) {
  ui_->l_MaterialShininess;
  ui_->materialShininess->setValue(component.shininess);
}

void MainWindow::SetTransformUi(Transform const &component) {
  ui_->xTrans->setValue(component.translation.x() * 100);
  ui_->yTrans->setValue(component.translation.y() * 100);
  ui_->zTrans->setValue(component.translation.z() * 100);
  ui_->xRot->setValue(component.rotation.x());
  ui_->yRot->setValue(component.rotation.y());
  ui_->zRot->setValue(component.rotation.z());
  ui_->xScale->setValue(component.scale.x());
  ui_->yScale->setValue(component.scale.y());
  ui_->zScale->setValue(component.scale.z());
}

void MainWindow::ConnectMaterialUi() {
  connect(ui_->materialShininess, &QDoubleSpinBox::valueChanged,
          [&](float value) {
            backend_->ChangeComponent<Material>(
                [value](auto &component) { component.shininess = value; });
          });
}

void MainWindow::ConnectLightUi() {
  connect(ui_->lightPositionX, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.position.setX(value); });
  });

  connect(ui_->lightPositionY, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.position.setY(value); });
  });

  connect(ui_->lightPositionZ, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.position.setZ(value); });
  });

  connect(ui_->lightDirectionX, &QDoubleSpinBox::valueChanged,
          [&](float value) {
            backend_->ChangeComponent<Light>(
                [value](auto &component) { component.direction.setX(value); });
          });

  connect(ui_->lightDirectionY, &QDoubleSpinBox::valueChanged,
          [&](float value) {
            backend_->ChangeComponent<Light>(
                [value](auto &component) { component.direction.setY(value); });
          });

  connect(ui_->lightDirectionZ, &QDoubleSpinBox::valueChanged,
          [&](float value) {
            backend_->ChangeComponent<Light>(
                [value](auto &component) { component.direction.setZ(value); });
          });

  connect(ui_->lightAmbientR, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.ambient.setX(value); });
  });

  connect(ui_->lightAmbientG, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.ambient.setY(value); });
  });
  connect(ui_->lightAmbientB, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.ambient.setZ(value); });
  });

  connect(ui_->lightDiffuseR, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.diffuse.setX(value); });
  });

  connect(ui_->lightDiffuseG, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.diffuse.setY(value); });
  });
  connect(ui_->lightDiffuseB, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.diffuse.setZ(value); });
  });

  connect(ui_->lightSpecularR, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.specular.setX(value); });
  });

  connect(ui_->lightSpecularG, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.specular.setY(value); });
  });
  connect(ui_->lightSpecularB, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.specular.setZ(value); });
  });

  connect(ui_->lightInnerCone, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.inner_cone = value; });
  });

  connect(ui_->lightOuterCone, &QDoubleSpinBox::valueChanged, [&](float value) {
    backend_->ChangeComponent<Light>(
        [value](auto &component) { component.outer_cone = value; });
  });
}

void MainWindow::ConnectTransformUi() {
  connect(ui_->xTrans, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>([value](auto &component) {
      component.translation.setX(value / 100.0);
    });
  });

  connect(ui_->yTrans, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>([value](auto &component) {
      component.translation.setY(value / 100.0);
    });
  });

  connect(ui_->zTrans, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>([value](auto &component) {
      component.translation.setZ(value / 100.0);
    });
  });

  connect(ui_->xRot, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>(
        [value](auto &component) { component.rotation.setX(value); });
  });

  connect(ui_->yRot, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>(
        [value](auto &component) { component.rotation.setY(value); });
  });

  connect(ui_->zRot, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>(
        [value](auto &component) { component.rotation.setZ(value); });
  });

  connect(ui_->xScale, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>(
        [value](auto &component) { component.scale.setX(value); });
  });

  connect(ui_->yScale, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>(
        [value](auto &component) { component.scale.setY(value); });
  });

  connect(ui_->zScale, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>(
        [value](auto &component) { component.scale.setZ(value); });
  });
}

}  // namespace s21
