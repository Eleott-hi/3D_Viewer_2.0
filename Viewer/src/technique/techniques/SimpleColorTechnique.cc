#include "SimpleColorTechnique.h"

#include <QDir>

namespace s21 {

void SimpleColorTechnique::Init() {
  GenerateShaders(":/shaders/simple_color_shader.vs",
                  ":/shaders/simple_color_shader.fs");
}

void SimpleColorTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                  QMatrix4x4 model) {
  shader_.setUniformValue("u_MVP", proj * view * model);
}

void SimpleColorTechnique::SetMaterial(Material const& material) {
  shader_.setUniformValue("material.color", material.color);
}

}  // namespace s21
