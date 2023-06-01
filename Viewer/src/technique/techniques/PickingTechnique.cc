#include "PickingTechnique.h"

namespace s21 {

void PickingTechnique::Init() {
  GenerateShaders(":/shaders/picking_shader.vs", ":/shaders/picking_shader.fs");
}

void PickingTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                              QMatrix4x4 model) {
  shader_.setUniformValue("u_MVP", projection_ * view_ * model);
}

void PickingTechnique::SetObjectID(int ObjectID) {
  shader_.setUniformValue("u_ObjectID", ObjectID);
}

}  // namespace s21
