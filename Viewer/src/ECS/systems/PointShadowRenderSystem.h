
#pragma once

#include <QOpenGLExtraFunctions>

#include "OpenGLFramebuffer.h"
#include "TechniqueStrategy.h"
#include "core/ECS_Controller.h"
#include "core/System.h"

namespace s21 {

class PointShadowRenderSystem : public System, protected QOpenGLExtraFunctions {
 public:
  PointShadowRenderSystem() { initializeOpenGLFunctions(); }
  ~PointShadowRenderSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update(uint32_t shadowMap);

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
};

}  // namespace s21
