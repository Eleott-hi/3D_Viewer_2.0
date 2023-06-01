#include "PointShadowRenderTechnique.h"

namespace s21 {

void PointShadowRenderTechnique::Init() {
  GenerateShaders(":/shaders/point_shadows.vs", ":/shaders/point_shadows.fs");
}

void PointShadowRenderTechnique::SetTexture(Texture const &texture) {
  shader_.setUniformValue(texture.type.c_str(), index_);
  glActiveTexture(GL_TEXTURE0 + index_);
  glBindTexture(index_ ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, texture.id);

  index_++;
}

void PointShadowRenderTechnique::SetMaterial(Material const &material) {
  SetTexture({material.ao.id, "material.aoMap"});
  SetTexture({material.normal.id, "material.normalMap"});
  SetTexture({material.diffuse.id, "material.albedoMap"});
  SetTexture({material.specular.id, "material.specularMap"});
  SetTexture({material.roughness.id, "material.roughnessMap"});
  shader_.setUniformValue("material.shininess", material.shininess);
}

void PointShadowRenderTechnique::OnMVPLoaded() {
  shader_.setUniformValue("View", view_);
  shader_.setUniformValue("Model", model_);
  shader_.setUniformValue("Projection", projection_);
  shader_.setUniformValue("camPos", QVector3D{view_.inverted().column(3)});
}

}  // namespace s21
