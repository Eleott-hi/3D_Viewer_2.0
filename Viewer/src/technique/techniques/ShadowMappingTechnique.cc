#include "ShadowMappingTechnique.h"

namespace s21 {

void ShadowMappingTechnique::Init() {
  GenerateShaders(":/shaders/shadow_mapping_depth.vs",
                  ":/shaders/shadow_mapping_depth.fs");
}

void ShadowMappingTechnique::SetTexture(Texture const &texture) {
  shader_.setUniformValue(texture.type.c_str(), index_);
  glActiveTexture(GL_TEXTURE0 + index_);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  index_++;
}

void ShadowMappingTechnique::OnMVPLoaded() {
  shader_.setUniformValue("View", view_);
  shader_.setUniformValue("Model", model_);
  shader_.setUniformValue("Projection", projection_);
}

}  // namespace s21
