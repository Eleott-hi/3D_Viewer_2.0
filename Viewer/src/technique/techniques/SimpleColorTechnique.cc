#include "SimpleColorTechnique.h"

#include <QDir>

namespace s21 {

void SimpleColorTechnique::Init() {
  GenerateShaders(":/shaders/simple_color_shader.vs",
                  ":/shaders/simple_color_shader.fs");
}

void SimpleColorTechnique::OnMVPLoaded() {
  shader_.setUniformValue("u_MVP", projection_ * view_ * model_);
}

void SimpleColorTechnique::SetMaterial(Material const& material) {
  shader_.setUniformValue("material.color", material.color);
}

}  // namespace s21
