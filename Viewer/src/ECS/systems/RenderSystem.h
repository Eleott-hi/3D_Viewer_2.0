
#pragma once

#include <QOpenGLExtraFunctions>

#include "TechniqueStrategy.h"
#include "core/ECS_Controller.h"
#include "core/System.h"
#include "OpenGLFramebuffer.h"

namespace s21 {

class RenderSystem : public System, protected QOpenGLExtraFunctions {
 public:
  RenderSystem() { initializeOpenGLFunctions(); }
  ~RenderSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
};
}  // namespace s21
