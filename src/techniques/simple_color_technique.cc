#include "simple_color_technique.h"

namespace s21 {

void SimpleColorTechnique::init() {
  GenerateShaders("shaders/simple_color_shader_v.c",
                  "shaders/simple_color_shader_f.c");
}

//   void SimpleColorTechnique::ApplySettings(ModelSettings *mSettings)  {
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

void SimpleColorTechnique::setMVP(const QMatrix4x4 &proj,
                                  const QMatrix4x4 &view,
                                  const QMatrix4x4 &model) {
  shader_.setUniformValue("u_MVP", proj * view * model);
}

void SimpleColorTechnique::setColor(const QColor &c) {
  shader_.setUniformValue("u_Color", c);
}

}  // namespace s21
