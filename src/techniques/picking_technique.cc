#include "picking_technique.h"

namespace s21 {

void PickingTechnique::init() {
  GenerateShaders("shaders/picking_shader_v.c", "shaders/picking_shader_f.c");
}

void PickingTechnique::setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                              const QMatrix4x4 &model) {
  shader_.setUniformValue("u_MVP", proj * view * model);
}

void PickingTechnique::SetObjectID(int ObjectID) {
  shader_.setUniformValue("u_ObjectID", ObjectID);
}

}  // namespace s21
