#include "quad_technique.h"

namespace s21 {

void QuadTechnique::init() {
  GenerateShaders(":/shaders/quad.vs", ":/shaders/quad.fs");
}

void QuadTechnique::setTextureID(uint32_t id) {
  shader_.setUniformValue("screenTexture", id);
}

}  // namespace s21
