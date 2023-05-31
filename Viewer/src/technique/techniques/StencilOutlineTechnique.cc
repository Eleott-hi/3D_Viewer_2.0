#include "StencilOutlineTechnique.h"

namespace s21 {

void StencilOutlineTechnique::Init() {
  GenerateShaders(":/shaders/stencil_shader.vs", ":/shaders/stencil_shader.fs");
}

void StencilOutlineTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                     QMatrix4x4 model) {
  shader_.setUniformValue("proj", proj);
  shader_.setUniformValue("view", view);
  shader_.setUniformValue("model", model);
}

}  // namespace s21
