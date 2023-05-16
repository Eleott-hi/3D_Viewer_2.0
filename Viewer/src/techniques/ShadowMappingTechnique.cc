#include "ShadowMappingTechnique.h"

namespace s21 {

void ShadowMappingTechnique::init() {
  GenerateShaders(":/shaders/shadow_mapping_depth.vs",
                  ":/shaders/shadow_mapping_depth.fs");
}

void ShadowMappingTechnique::setTexture(Texture const &texture) {
  shader_.setUniformValue(texture.type.c_str(), index_);
  glActiveTexture(GL_TEXTURE0 + index_);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  index_++;
}

void ShadowMappingTechnique::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                    QMatrix4x4 model) {
  shader_.setUniformValue("Projection", proj);
  shader_.setUniformValue("View", view);
  shader_.setUniformValue("Model", model);
}

}  // namespace s21