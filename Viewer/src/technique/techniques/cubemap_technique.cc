#include "cubemap_technique.h"

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

void CubemapTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                              QMatrix4x4 model) {
  Q_UNUSED(model);

  shader_.setUniformValue("proj", proj);

  view(0, 3) = 0;
  view(1, 3) = 0;
  view(2, 3) = 0;
  view(3, 3) = 1;

  shader_.setUniformValue("view", view);
}

}  // namespace s21
