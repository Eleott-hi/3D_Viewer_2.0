#include "simple_texture_technique.h"

namespace s21 {

void SimpleTextureTechnique::init() {
  GenerateShaders(":/shaders/simple_texture_shader.vs",
                  ":/shaders/simple_texture_shader.fs");
}

void SimpleTextureTechnique::setTextureID(uint32_t id) {
  // glActiveTexture(GL_TEXTURE0 + 0);

  shader_.setUniformValue("diffuse", id);
  // glBindTexture(GL_TEXTURE_2D, texture.diffuse);
}

void SimpleTextureTechnique::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                    QMatrix4x4 model) {
  shader_.setUniformValue("u_MVP", proj * view * model);
}

}  // namespace s21
