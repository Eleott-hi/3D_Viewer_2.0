#include "SimpleTextureTechnique.h"

namespace s21 {

void SimpleTextureTechnique::Init() {
  GenerateShaders(":/shaders/simple_texture_shader.vs",
                  ":/shaders/simple_texture_shader.fs");
}

void SimpleTextureTechnique::SetTexture(Texture const &texture) {
  shader_.setUniformValue(texture.type.c_str(), 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

void SimpleTextureTechnique::SetMaterial(Material const &material) {
  SetTexture({material.diffuse.id, "diffuse"});
  shader_.setUniformValue("u_material.shininess", material.shininess);
}

void SimpleTextureTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                    QMatrix4x4 model) {
  shader_.setUniformValue("u_MVP", projection_ * view_ * model);
}

}  // namespace s21
