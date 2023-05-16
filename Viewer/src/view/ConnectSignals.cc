#include <QColorDialog>
#include <QFileDialog>

#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace s21 {

void MainWindow::ConnectSignals() {
  ConnectCameraUi();
  ConnectTransformUi();
  ConnectLightUi();
  ConnectMaterialUi();
  ConnectShaderUi();
}

void MainWindow::SetLightUi(Light const &component) {
  ui_->cb_LightType->setCurrentIndex((int)component.type);
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
  ui_->l_MaterialColor->setAutoFillBackground(true);
  ui_->l_MaterialColor->setPalette({component.color});

  ui_->l_MaterialDiffuseFilename->setText(component.diffuse.filename);
  ui_->l_MaterialDiffuseImage->setPixmap(QPixmap::fromImage(
      component.diffuse.image.scaled(ui_->l_MaterialDiffuseImage->size())));

  ui_->l_MaterialSpecularFilename->setText(component.specular.filename);
  ui_->l_MaterialSpecularImage->setPixmap(QPixmap::fromImage(
      component.specular.image.scaled(ui_->l_MaterialSpecularImage->size())));

  ui_->l_MaterialNormalFilename->setText(component.normal.filename);
  ui_->l_MaterialNormalImage->setPixmap(QPixmap::fromImage(
      component.normal.image.scaled(ui_->l_MaterialNormalImage->size())));

  ui_->l_MaterialRoughnessFilename->setText(component.roughness.filename);
  ui_->l_MaterialRoughnessImage->setPixmap(QPixmap::fromImage(
      component.roughness.image.scaled(ui_->l_MaterialRoughnessImage->size())));

  ui_->l_MaterialMetallicFilename->setText(component.metallic.filename);
  ui_->l_MaterialMetallicImage->setPixmap(QPixmap::fromImage(
      component.metallic.image.scaled(ui_->l_MaterialMetallicImage->size())));

  ui_->l_MaterialAoFilename->setText(component.ao.filename);
  ui_->l_MaterialAoImage->setPixmap(QPixmap::fromImage(
      component.ao.image.scaled(ui_->l_MaterialAoImage->size())));

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

void MainWindow::SetShaderUi(Shader const &component) {
  ui_->cb_ShaderType->setCurrentIndex((int)component.type);
}

void MainWindow::ConnectShaderUi() {
  ui_->tb_Render->setAutoExclusive(false);

  connect(ui_->tb_Render, &QRadioButton::clicked, [this](bool toggle) {
    toggle ? backend_->AddComponent<RenderTag>()
           : backend_->RemoveComponent<RenderTag>();

    Notify();
  });

  connect(ui_->cb_ShaderType, &QComboBox::currentIndexChanged, [&](int index) {
    backend_->ChangeComponent<Shader>(
        [index](auto &component) { component.type = (TechniqueType)index; });
  });
}

void MainWindow::ConnectMaterialUi() {
  connect(ui_->pb_MaterialColorChange, &QPushButton::pressed, [&]() {
    QColor color = QColorDialog::getColor();
    if (!color.isValid()) return;

    ui_->l_MaterialColor->setPalette({color});

    backend_->ChangeComponent<Material>(
        [color](auto &component) { component.color = color; });
  });

  connect(ui_->materialShininess, &QDoubleSpinBox::valueChanged,
          [&](float value) {
            backend_->ChangeComponent<Material>(
                [value](auto &component) { component.shininess = value; });
          });

  connect(ui_->pb_MaterialDiffuseChange, &QPushButton::clicked, [&]() {
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg)"));

    if (filename.isEmpty()) return;

    backend_->ChangeComponent<Material>([&](auto &component) {
      backend_->LoadTexture(filename, component.diffuse);
    });

    Notify();
  });

  connect(ui_->pb_MaterialSpecularChange, &QPushButton::clicked, [&]() {
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg)"));

    if (filename.isEmpty()) return;

    backend_->ChangeComponent<Material>([&](auto &component) {
      backend_->LoadTexture(filename, component.specular);
    });

    Notify();
  });

  connect(ui_->pb_MaterialNormalChange, &QPushButton::clicked, [&]() {
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg)"));

    if (filename.isEmpty()) return;

    backend_->ChangeComponent<Material>([&](auto &component) {
      backend_->LoadTexture(filename, component.normal);
    });

    Notify();
  });

  connect(ui_->pb_MaterialRoughnessChange, &QPushButton::clicked, [&]() {
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg)"));

    if (filename.isEmpty()) return;

    backend_->ChangeComponent<Material>([&](auto &component) {
      backend_->LoadTexture(filename, component.roughness);
    });

    Notify();
  });

  connect(ui_->pb_MaterialMetallicChange, &QPushButton::clicked, [&]() {
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg)"));

    if (filename.isEmpty()) return;

    backend_->ChangeComponent<Material>([&](auto &component) {
      backend_->LoadTexture(filename, component.metallic);
    });

    Notify();
  });

  connect(ui_->pb_MaterialAoChange, &QPushButton::clicked, [&]() {
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg)"));

    if (filename.isEmpty()) return;

    backend_->ChangeComponent<Material>([&](auto &component) {
      backend_->LoadTexture(filename, component.ao);
    });

    Notify();
  });
}

void MainWindow::ConnectLightUi() {
  ui_->tg_LightSource->setAutoExclusive(false);

  connect(ui_->tg_LightSource, &QRadioButton::clicked, [this](bool toggle) {
    toggle ? backend_->AddComponent<Light>()
           : backend_->RemoveComponent<Light>();

    Notify();
  });

  connect(ui_->cb_LightType, &QComboBox::currentIndexChanged, [&](int index) {
    backend_->ChangeComponent<Light>(
        [index](auto &component) { component.type = (LightType)index; });
  });

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
  ui_->rb_ScaleProportional->setAutoExclusive(false);

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
    if (ui_->rb_ScaleProportional->isChecked()) {
      ui_->yScale->setValue(value);
      ui_->zScale->setValue(value);
    }
  });

  connect(ui_->yScale, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>(
        [value](auto &component) { component.scale.setY(value); });
    if (ui_->rb_ScaleProportional->isChecked()) {
      ui_->xScale->setValue(value);
      ui_->zScale->setValue(value);
    }
  });

  connect(ui_->zScale, &QSlider::valueChanged, [&](float value) {
    backend_->ChangeComponent<Transform>(
        [value](auto &component) { component.scale.setZ(value); });
    if (ui_->rb_ScaleProportional->isChecked()) {
      ui_->xScale->setValue(value);
      ui_->yScale->setValue(value);
    }
  });
}

void MainWindow::ConnectCameraUi() {
  connect(

      ui_->dsb_CameraTransX, &QDoubleSpinBox::valueChanged, [&](float value) {
        static auto &camera = backend_->GetCamera();
        camera.position.setX(value);
      });

  connect(ui_->dsb_CameraTransY, &QDoubleSpinBox::valueChanged,
          [&](float value) {
            static auto &camera = backend_->GetCamera();
            camera.position.setY(value);
          });

  connect(ui_->dsb_CameraTransZ, &QDoubleSpinBox::valueChanged,
          [&](float value) {
            static auto &camera = backend_->GetCamera();
            camera.position.setZ(value);
          });

  connect(ui_->dsb_CameraYaw, &QDoubleSpinBox::valueChanged, [&](float value) {
    static auto &camera = backend_->GetCamera();
    camera.yaw = value;
  });

  connect(ui_->dsb_CameraPitch, &QDoubleSpinBox::valueChanged,
          [&](float value) {
            static auto &camera = backend_->GetCamera();
            camera.pitch = value;
          });
}

}  // namespace s21
