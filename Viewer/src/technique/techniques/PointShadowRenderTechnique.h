#pragma once

#include "ITechnique.h"

namespace s21 {
class PointShadowRenderTechnique : public ITechnique {
 public:
  PointShadowRenderTechnique() { Init(); }
  ~PointShadowRenderTechnique() = default;

  void Init() final {
    GenerateShaders(":/shaders/point_shadows.vs", ":/shaders/point_shadows.fs");
  }

  void SetTexture(Texture const &texture) final {
    shader_.setUniformValue(texture.type.c_str(), index_);
    glActiveTexture(GL_TEXTURE0 + index_);
    glBindTexture(index_ ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, texture.id);

    index_++;
  }

  void SetMaterial(Material const &material) {
    SetTexture({material.ao.id, "material.aoMap"});
    SetTexture({material.normal.id, "material.normalMap"});
    SetTexture({material.diffuse.id, "material.albedoMap"});
    SetTexture({material.specular.id, "material.specularMap"});
    SetTexture({material.roughness.id, "material.roughnessMap"});
    shader_.setUniformValue("material.shininess", material.shininess);
  }

  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final {
    shader_.setUniformValue("Projection", projection_);
    shader_.setUniformValue("View", view_);
    shader_.setUniformValue("Model", model);

    shader_.setUniformValue("camPos", QVector3D{view_.inverted().column(3)});
  }
};
}  // namespace s21
