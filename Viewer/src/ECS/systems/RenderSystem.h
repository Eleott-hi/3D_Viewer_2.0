
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "framebuffer.h"
#include "technique_strategy.h"

namespace s21 {

class RenderSystem : public System, protected QOpenGLExtraFunctions {
 public:
  RenderSystem();
  ~RenderSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

 private:
  ECS_Controller *scene_ = nullptr;
  QMatrix4x4 proj_ ;
  QMatrix4x4 view_ ;
  QMatrix4x4 model_;

  TechniqueStrategy *technique_ = nullptr;
  std::unique_ptr<IFramebuffer> framebuffer_ = std::make_unique<Framebuffer>();

  void DrawObject(NewModel &model, GLenum form = GL_TRIANGLES);
  void DrawMesh(Mesh &mesh, GLenum form);

      void OnWindowResize(Event &e);
  void PrepareFramebuffer();
};
}  // namespace s21
