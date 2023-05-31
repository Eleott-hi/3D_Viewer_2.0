#include "DefferedShadingTechnique.h"

#include <string>

namespace s21 {

void DefferedShadingTechnique::Init() {
  GenerateShaders(":/shaders/g_buffer.vs", ":/shaders/g_buffer.fs");
}

void DefferedShadingTechnique::SetTexture(Texture const &texture) {
  shader_.setUniformValue(texture.type.c_str(), index_);
  glActiveTexture(GL_TEXTURE0 + index_);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  index_++;
}

void DefferedShadingTechnique::SetMaterial(Material const &material) {
  SetTexture({material.diffuse.id, "diffuse"});
  SetTexture({material.specular.id, "specular"});
}

void DefferedShadingTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                      QMatrix4x4 model) {
  shader_.setUniformValue("view", view);
  shader_.setUniformValue("model", model);
  shader_.setUniformValue("projection", proj);
}

}  // namespace s21
