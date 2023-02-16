
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/Controller.h"
#include "core/System.h"
#include "technique_strategy.h"

namespace s21 {

class RenderSystem : public System, protected QOpenGLExtraFunctions {
 public:
  RenderSystem() { initializeOpenGLFunctions(); }
  ~RenderSystem() = default;

  void Init(std::shared_ptr<Controller> const &scene,
            std::shared_ptr<TechniqueStrategy> const &technique);
  void Update(EntityID camera);

 private:
  std::shared_ptr<Controller> scene_;
  std::shared_ptr<TechniqueStrategy> technique_;

  void UpdateLightData();
  void UpdateGeometrySettings(PointSettingsComponent const &pointSettings,
                              LineSettingsComponent const &lineSettings);
  void NormalizeOpenGLSettings();

  void DrawObject(MeshComponent &model, GLenum form = GL_TRIANGLES);
  void bufferize(s_Mesh &mesh);
};
}  // namespace s21
