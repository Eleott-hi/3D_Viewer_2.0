
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "technique_strategy.h"

namespace s21 {

class RenderPickedSystem : public System, protected QOpenGLExtraFunctions {
 public:
  RenderPickedSystem() { initializeOpenGLFunctions(); }
  ~RenderPickedSystem() = default;

  void Init(std::shared_ptr<ECS_Controller> const &scene,
            std::shared_ptr<TechniqueStrategy> const &technique);

  void Update(EntityID camera);

 private:
  std::shared_ptr<ECS_Controller> scene_;
  std::shared_ptr<TechniqueStrategy> technique_;

  void DrawObject(MeshComponent &model);
  void Bufferize(s_Mesh &mesh);
};
}  // namespace s21
