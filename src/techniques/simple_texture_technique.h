#ifndef SRC_HEADERS_SIMPLE_TEXTURE_H
#define SRC_HEADERS_SIMPLE_TEXTURE_H

#include <QOpenGLExtraFunctions>

#include "i_technique.h"

namespace s21 {

class SimpleTextureTechnique : public ITechnique,
                               protected QOpenGLExtraFunctions {
 public:
  SimpleTextureTechnique() {
    initializeOpenGLFunctions();
    init();
  }
  ~SimpleTextureTechnique() {}

  virtual void init() override {
    GenVertexAndFragmentShader(shader_, "shaders/simple_texture_shader_v.c",
                               "shaders/simple_texture_shader_f.c");
  }

  virtual void Enable() override { shader_.bind(); }



  virtual void setTexture(TextureComponent const& texture) override {
    glActiveTexture(GL_TEXTURE0 + 0);
    shader_.setUniformValue("diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, texture.diffuse);
  }

  virtual void setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                      const QMatrix4x4 &model) override {
    shader_.setUniformValue("u_MVP", proj * view * model);
  }

 private:
  QOpenGLShaderProgram shader_;
};

}  // namespace s21

#endif  // SRC_HEADERS_SIMPLE_TEXTURE_H
