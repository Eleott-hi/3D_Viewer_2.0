#include "quad_technique.h"

namespace s21 {

void QuadTechnique::init() {
  GenerateShaders(":/shaders/quad.vs", ":/shaders/quad.fs");
}

void QuadTechnique::setTexture(Texture const &texture) {
  auto const &[id, type] = texture;
  shader_.setUniformValue(type.c_str(), index_);
  glActiveTexture(GL_TEXTURE0 + index_);
  glBindTexture(GL_TEXTURE_2D, id);

  index_++;
}

}  // namespace s21
