
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "framebuffer.h"
#include "technique_strategy.h"

namespace s21 {

class PointShadowSystem : public System, protected QOpenGLExtraFunctions {
 public:
  PointShadowSystem() { initializeOpenGLFunctions(); }
  ~PointShadowSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

  unsigned int depthCubemap;

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;

  unsigned int depthMapFBO;
};

}  // namespace s21
