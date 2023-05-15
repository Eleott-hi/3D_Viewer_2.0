#include "simple_texture_technique.h"

namespace s21 {

void SimpleTextureTechnique::init() {
  GenerateShaders(":/shaders/simple_texture_shader.vs",
                  ":/shaders/simple_texture_shader.fs");
}

void SimpleTextureTechnique::setTexture(Texture const &texture) {
  shader_.setUniformValue(texture.type.c_str(), 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

void SimpleTextureTechnique::setMaterial(Material const &material) {
  setTexture({material.diffuse.id, "diffuse"});
  shader_.setUniformValue("u_material.shininess", material.shininess);
}

void SimpleTextureTechnique::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                    QMatrix4x4 model) {
  shader_.setUniformValue("u_MVP", proj * view * model);
}

}  // namespace s21
