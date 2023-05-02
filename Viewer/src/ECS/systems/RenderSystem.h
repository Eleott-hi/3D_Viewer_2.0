
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "technique_strategy.h"

namespace s21 {

class RenderSystem : public System, protected QOpenGLExtraFunctions {
 public:
  RenderSystem() { initializeOpenGLFunctions(); }
  ~RenderSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

 private:
  ECS_Controller *scene_;
  TechniqueStrategy *technique_;

  void DrawObject(Model &model, GLenum form = GL_TRIANGLES);
};
}  // namespace s21
