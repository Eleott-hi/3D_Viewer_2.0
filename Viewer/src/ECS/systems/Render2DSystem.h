
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "technique_strategy.h"

namespace s21 {

class Render2DSystem : public System, protected QOpenGLExtraFunctions {
 public:
  Render2DSystem() { initializeOpenGLFunctions(); }
  ~Render2DSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update(uint32_t pos, uint32_t normal, uint32_t albedo_spec);

 private:
  ECS_Controller *scene_;
  TechniqueStrategy *technique_;

  void renderQuad();
};
}  // namespace s21
