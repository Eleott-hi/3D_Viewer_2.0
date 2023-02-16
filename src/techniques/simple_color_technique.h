#ifndef SRC_HEADERS_SIMPLE_COLOR_H
#define SRC_HEADERS_SIMPLE_COLOR_H

#include "i_technique.h"

namespace s21 {

class SimpleColorTechnique : public ITechnique {
 public:
  SimpleColorTechnique() { init(); }
  ~SimpleColorTechnique() {}

  virtual void init() override {
    GenVertexAndFragmentShader(shader_, "shaders/simple_color_shader_v.c",
                               "shaders/simple_color_shader_f.c");
  }

  virtual void Enable() override { shader_.bind(); }

//  virtual void ApplySettings(ModelSettings *mSettings) override {
//    mSettings->getPointForm() ? glEnable(GL_POINT_SMOOTH)
//                              : glDisable(GL_POINT_SMOOTH);
//    mSettings->getLinesForm() ? glEnable(GL_LINE_STIPPLE)
//                              : glDisable(GL_LINE_STIPPLE);
//    mSettings->getFillFaces() ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
//                              : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glLineWidth(mSettings->getLineWidth());
//    glPointSize(mSettings->getPointSize());

//    shader_.setUniformValue("u_Color", mSettings->getVertexColor());
//  }

  virtual void setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                      const QMatrix4x4 &model) override {
    shader_.setUniformValue("u_MVP", proj * view * model);
  }

  virtual void setColor(const QColor &c) override {
    shader_.setUniformValue("u_Color", c);
  }

 private:
  QOpenGLShaderProgram shader_;
};

}  // namespace s21

#endif  // SRC_HEADERS_SIMPLE_COLOR_H
