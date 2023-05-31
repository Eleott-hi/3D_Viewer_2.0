#include "QuadTechnique.h"

namespace s21 {

void QuadTechnique::Init() {
  GenerateShaders(":/shaders/quad.vs", ":/shaders/quad.fs");
}

void QuadTechnique::SetTexture(Texture const &texture) {
  shader_.setUniformValue(texture.type.c_str(), index_);
  glActiveTexture(GL_TEXTURE0 + index_);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  index_++;
}

}  // namespace s21
