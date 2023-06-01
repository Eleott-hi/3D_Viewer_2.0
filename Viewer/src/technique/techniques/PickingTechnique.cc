#include "PickingTechnique.h"

namespace s21 {

void PickingTechnique::Init() {
  GenerateShaders(":/shaders/picking_shader.vs", ":/shaders/picking_shader.fs");
}

void PickingTechnique::OnMVPLoaded() {
  shader_.setUniformValue("u_MVP", projection_ * view_ * model_);
}

void PickingTechnique::SetObjectID(int ObjectID) {
  shader_.setUniformValue("u_ObjectID", ObjectID);
}

}  // namespace s21
