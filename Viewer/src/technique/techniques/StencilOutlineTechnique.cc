#include "StencilOutlineTechnique.h"

namespace s21 {

void StencilOutlineTechnique::Init() {
  GenerateShaders(":/shaders/stencil_shader.vs", ":/shaders/stencil_shader.fs");
}

void StencilOutlineTechnique::OnMVPLoaded() {
  shader_.setUniformValue("view", view_);
  shader_.setUniformValue("model", model_);
  shader_.setUniformValue("proj", projection_);
}

}  // namespace s21
