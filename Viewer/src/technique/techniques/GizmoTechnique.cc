#include "GizmoTechnique.h"

namespace s21 {

void GizmoTechnique::Init() {
  GenerateShaders(":/shaders/gizmo.vs", ":/shaders/gizmo.fs");
}

void GizmoTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                            QMatrix4x4 model) {
  shader_.setUniformValue("u_mvp", proj * view * model);

  auto tmp = view.inverted();
  shader_.setUniformValue("u_eye", QVector3D{tmp(0, 3), tmp(1, 3), tmp(2, 3)});
}

}  // namespace s21
