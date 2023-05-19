
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "framebuffer.h"
#include "technique_strategy.h"

namespace s21 {

class ShadowRenderSystem : public System, protected QOpenGLExtraFunctions {
 public:
  ShadowRenderSystem() { initializeOpenGLFunctions(); }
  ~ShadowRenderSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update(uint32_t shadowMap);

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
};

}  // namespace s21
