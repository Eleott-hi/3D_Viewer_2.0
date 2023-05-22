#pragma once

#include "i_technique.h"

namespace s21 {
class PointShadowRenderTechnique : public ITechnique {
 public:
  PointShadowRenderTechnique() { init(); }
  ~PointShadowRenderTechnique() = default;

  void init() final {
    GenerateShaders(":/shaders/point_shadows.vs", ":/shaders/point_shadows.fs");
  }

  void Clear() final { index_ = 0; }

  void setTexture(Texture const &texture) final {
    shader_.setUniformValue(texture.type.c_str(), index_);
    glActiveTexture(GL_TEXTURE0 + index_);
    glBindTexture(index_ ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, texture.id);

    index_++;
  }

  void setMaterial(Material const &material) {
    setTexture({material.ao.id, "material.aoMap"});
    setTexture({material.normal.id, "material.normalMap"});
    setTexture({material.diffuse.id, "material.albedoMap"});
    setTexture({material.specular.id, "material.specularMap"});
    setTexture({material.roughness.id, "material.roughnessMap"});
    shader_.setUniformValue("material.shininess", material.shininess);
  }

  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final {
    shader_.setUniformValue("Projection", proj);
    shader_.setUniformValue("View", view);
    shader_.setUniformValue("Model", model);

    auto tmp = view.inverted();
    shader_.setUniformValue("camPos",
                            QVector3D{tmp(0, 3), tmp(1, 3), tmp(2, 3)});
  }

 private:
  uint32_t index_ = 0;
};
}  // namespace s21
