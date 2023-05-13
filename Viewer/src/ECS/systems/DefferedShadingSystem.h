
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "technique_strategy.h"

namespace s21 {

class DefferedShadingSystem : public System, protected QOpenGLExtraFunctions {
 public:
  DefferedShadingSystem() { initializeOpenGLFunctions(); }
  ~DefferedShadingSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
};
}  // namespace s21
