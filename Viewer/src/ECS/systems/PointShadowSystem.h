
#pragma once

#include <QOpenGLExtraFunctions>

#include "TechniqueStrategy.h"
#include "core/ECS_Controller.h"
#include "core/System.h"
#include "OpenGLFramebuffer.h"

namespace s21 {

class PointShadowSystem : public System, protected QOpenGLExtraFunctions {
 public:
  PointShadowSystem() { initializeOpenGLFunctions(); }
  ~PointShadowSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
};

}  // namespace s21
