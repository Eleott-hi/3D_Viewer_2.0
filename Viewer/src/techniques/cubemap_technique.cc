#include "cubemap_technique.h"

#include <QDir>
#include <QMatrix3x3>

namespace s21 {

void CubemapTechnique::init() {
  GenerateShaders(":/shaders/cubemap.vs", ":/shaders/cubemap.fs");
}

void CubemapTechnique::setTextureID(uint32_t id) {
  shader_.setUniformValue("skybox", id);
}

void CubemapTechnique::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                              QMatrix4x4 model) {
  Q_UNUSED(model);

  shader_.setUniformValue("proj", proj);

  view(0, 3) = 0;
  view(1, 3) = 0;
  view(2, 3) = 0;

  shader_.setUniformValue("view", view);
}

}  // namespace s21
