
#pragma once

#include <QColor>
#include <QOpenGLExtraFunctions>
#include <functional>

#include "components/Components.h"
#include "core/ECS_Controller.h"
#include "core/System.h"
#include "signal_handler.h"

namespace s21 {

class ObjectEditorSystem : public System, protected QOpenGLExtraFunctions {
 public:
  ~ObjectEditorSystem() = default;
  ObjectEditorSystem() { initializeOpenGLFunctions(); }
  void Init(std::shared_ptr<ECS_Controller> const &scene) { scene_ = scene; }

  void ObjectSelected();
  void UpdateTransformInfo(TransformComponent const &t);
  void UpdatePointInfo(PointSettingsComponent const &settings);
  void UpdateLineInfo(LineSettingsComponent const &settings);
  void UpdateAxisInfo(bool axis);
  void UpdateTextureTechnique(TechniqueType type);
  void UpdateTextureInfo(uint32_t textureID, TextureType type);
  void MakeLightSource(bool light);
  void UpdatePointLightInfo(LightSettingsComponent const &settings);
  void UpdateMaterialInfo(MaterialComponent const &value);
  void DeleteObject();

 private:
  std::shared_ptr<ECS_Controller> scene_;
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();
};
}  // namespace s21
