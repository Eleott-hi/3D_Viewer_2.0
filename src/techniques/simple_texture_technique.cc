#include "simple_texture_technique.h"

namespace s21 {

SimpleTextureTechnique::SimpleTextureTechnique() {
  initializeOpenGLFunctions();
  init();
}

void SimpleTextureTechnique::init() {
  GenVertexAndFragmentShader(shader_, "shaders/simple_texture_shader_v.c",
                             "shaders/simple_texture_shader_f.c");
}

void SimpleTextureTechnique::setTexture(TextureComponent const &texture) {
  glActiveTexture(GL_TEXTURE0 + 0);
  shader_.setUniformValue("diffuse", 0);
  glBindTexture(GL_TEXTURE_2D, texture.diffuse);
}

void SimpleTextureTechnique::setMVP(const QMatrix4x4 &proj,
                                    const QMatrix4x4 &view,
                                    const QMatrix4x4 &model) {
  shader_.setUniformValue("u_MVP", proj * view * model);
}

}  // namespace s21
