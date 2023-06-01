#include "GizmoTechnique.h"

namespace s21 {

void GizmoTechnique::Init() {
  GenerateShaders(":/shaders/gizmo.vs", ":/shaders/gizmo.fs");
}

void GizmoTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                            QMatrix4x4 model) {
  shader_.setUniformValue("u_mvp", projection_ * view_ * model);

  shader_.setUniformValue("u_eye", QVector3D{view_.inverted().column(3)});
}

}  // namespace s21
