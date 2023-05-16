
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "framebuffer.h"
#include "technique_strategy.h"

namespace s21 {

class ShadowSystem : public System, protected QOpenGLExtraFunctions {
 public:
  ShadowSystem() { initializeOpenGLFunctions(); }
  ~ShadowSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

  uint32_t depthMap;

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
  uint32_t depthMapFBO;
};

}  // namespace s21
