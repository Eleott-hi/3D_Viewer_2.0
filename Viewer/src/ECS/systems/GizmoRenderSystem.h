
#pragma once

#include <QOpenGLExtraFunctions>

#include "core/ECS_Controller.h"
#include "core/System.h"
#include "gizmo/tiny-gizmo.h"
#include "technique_strategy.h"

namespace s21 {

class GizmoRenderSystem : public System, protected QOpenGLExtraFunctions {
 public:
  GizmoRenderSystem() { initializeOpenGLFunctions(); }
  ~GizmoRenderSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;

  tinygizmo::gizmo_application_state gizmo_state;
  tinygizmo::gizmo_context gizmo_ctx;

  void RenderGizmo(const tinygizmo::geometry_mesh &r);
};
}  // namespace s21
