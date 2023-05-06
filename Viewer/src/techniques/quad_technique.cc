#include "quad_technique.h"

namespace s21 {

void QuadTechnique::init() {
  GenerateShaders(":/shaders/quad.vs", ":/shaders/quad.fs");
}

void QuadTechnique::setTexture(Texture const &texture) {
  auto const &[id, type] = texture;
  shader_.setUniformValue("screenTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id);
}

}  // namespace s21
