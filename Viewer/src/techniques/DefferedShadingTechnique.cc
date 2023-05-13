#include "DefferedShadingTechnique.h"

#include <string>

namespace s21 {

void DefferedShadingTechnique::init() {
  GenerateShaders(":/shaders/g_buffer.vs", ":/shaders/g_buffer.fs");
}

void DefferedShadingTechnique::setTexture(Texture const &texture) {
  auto const &[id, type] = texture;
  shader_.setUniformValue(type.c_str(), texture_index_);
  glActiveTexture(GL_TEXTURE0 + texture_index_);
  glBindTexture(GL_TEXTURE_2D, id);

  texture_index_++;
}

void DefferedShadingTechnique::setMaterial(Material const &material) {
  setTexture({material.diffuse, "diffuse"});
  setTexture({material.specular, "specular"});
}

void DefferedShadingTechnique::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                      QMatrix4x4 model) {
  shader_.setUniformValue("view", view);
  shader_.setUniformValue("model", model);
  shader_.setUniformValue("projection", proj);
}

}  // namespace s21
