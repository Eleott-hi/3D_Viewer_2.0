#include "CubemapTechnique.h"

#include <QDir>
#include <QMatrix3x3>

namespace s21 {

void CubemapTechnique::Init() {
  GenerateShaders(":/shaders/cubemap.vs", ":/shaders/cubemap.fs");
}

void CubemapTechnique::SetTexture(Texture const &texture) {
  shader_.setUniformValue("skybox", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);
}

void CubemapTechnique::OnMVPLoaded() {
  auto tmp = view_;
  tmp.setColumn(3, {0, 0, 0, 1});
  shader_.setUniformValue("view", tmp);
  shader_.setUniformValue("proj", projection_);
}

}  // namespace s21
