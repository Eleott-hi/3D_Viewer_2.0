
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "technique_strategy.h"

namespace s21 {

class LightSystem : public System, protected QOpenGLExtraFunctions {
 public:
  LightSystem() { initializeOpenGLFunctions(); }
  ~LightSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

 private:
  ECS_Controller *scene_;
  TechniqueStrategy *technique_;

};
}  // namespace s21
