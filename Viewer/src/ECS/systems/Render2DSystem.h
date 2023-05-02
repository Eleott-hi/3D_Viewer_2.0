
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
  void Update();

 private:
  ECS_Controller *scene_;
  TechniqueStrategy *technique_;

  void renderQuad();
};
}  // namespace s21
