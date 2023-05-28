#include "picking_technique.h"

namespace s21 {

void PickingTechnique::init() {
  GenerateShaders(":/shaders/picking_shader.vs", ":/shaders/picking_shader.fs");
}

void PickingTechnique::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                              QMatrix4x4 model) {
  shader_.setUniformValue("u_MVP", proj * view * model);
}

void PickingTechnique::SetObjectID(int ObjectID) {
  shader_.setUniformValue("u_ObjectID", ObjectID);
}

}  // namespace s21
