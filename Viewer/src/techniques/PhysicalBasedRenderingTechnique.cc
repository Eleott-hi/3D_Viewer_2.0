#include "PhysicalBasedRenderingTechnique.h"

#include "Utils.h"

namespace s21 {

void PhysicalBasedRenderingTechnique::init() {
  GenerateShaders(":/shaders/PBR.vs", ":/shaders/PBR.fs");
}
void PhysicalBasedRenderingTechnique::setTexture(Texture const &texture) {
  auto const &[id, type] = texture;

  shader_.setUniformValue(type.c_str(), index_);
  glActiveTexture(GL_TEXTURE0 + index_);
  glBindTexture(GL_TEXTURE_2D, id);

  index_++;
}

void PhysicalBasedRenderingTechnique::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                             QMatrix4x4 model) {
  shader_.setUniformValue("Model", model);
  shader_.setUniformValue("View", view);
  shader_.setUniformValue("Projection", proj);

  auto tmp = view.inverted();
  shader_.setUniformValue("camPos", QVector3D{tmp(0, 3), tmp(1, 3), tmp(2, 3)});
}

void PhysicalBasedRenderingTechnique::setMaterial(Material const &material) {
  setTexture({material.ao, "material.aoMap"});
  setTexture({material.normal, "material.normalMap"});
  setTexture({material.diffuse, "material.albedoMap"});
  setTexture({material.specular, "material.specularMap"});
  setTexture({material.roughness, "material.roughnessMap"});
  shader_.setUniformValue("material.shininess", material.shininess);
}

void PhysicalBasedRenderingTechnique::SetLightComponent(
    QOpenGLShaderProgram &shader, std::string const &type, Light const &light) {
  shader.setUniformValue(Utils::StructName(type, "ambient").c_str(),
                         light.ambient);
  shader.setUniformValue(Utils::StructName(type, "diffuse").c_str(),
                         light.diffuse);
  shader.setUniformValue(Utils::StructName(type, "specular").c_str(),
                         light.specular);
}

void PhysicalBasedRenderingTechnique::SetAttenuationComponent(
    QOpenGLShaderProgram &shader, std::string const &type,
    Attenuation const &attenuation) {
  auto const &[constant, linear, quadratic] = attenuation;
  shader.setUniformValue(Utils::StructName(type, "linear").c_str(), linear);
  shader.setUniformValue(Utils::StructName(type, "constant").c_str(), constant);
  shader.setUniformValue(Utils::StructName(type, "quadratic").c_str(),
                         quadratic);
}

void PhysicalBasedRenderingTechnique::SetLightSpecificComponent(
    QOpenGLShaderProgram &shader, std::string const &type, Light const &light,
    int light_index, int attenuation_index) {
  shader.setUniformValue(Utils::StructName(type, "position").c_str(),
                         light.position);
  shader.setUniformValue(Utils::StructName(type, "direction").c_str(),
                         light.direction);
  shader.setUniformValue(Utils::StructName(type, "inner_cone").c_str(),
                         qCos(qDegreesToRadians(light.inner_cone)));
  shader.setUniformValue(Utils::StructName(type, "outer_cone").c_str(),
                         qCos(qDegreesToRadians(light.outer_cone)));
  shader.setUniformValue(Utils::StructName(type, "light_index").c_str(),
                         light_index);
  shader.setUniformValue(Utils::StructName(type, "attenuation_index").c_str(),
                         attenuation_index);
}

void PhysicalBasedRenderingTechnique::setLight(
    QVector<Light> lights, QVector<std::optional<Attenuation>> attenuations) {
  int dirLightsCount = 0, pointLightsCount = 0, spotLightsCount = 0;

  auto getArrayName = [&](Light const &light) -> std::string {
    if (light.type == LightType::DIRECTIONAL)
      return Utils::ArrayName("dirLights", dirLightsCount++);

    if (light.type == LightType::POINT)
      return Utils::ArrayName("pointLights", pointLightsCount++);

    if (light.type == LightType::SPOT)
      return Utils::ArrayName("spotLights", spotLightsCount++);

    Q_ASSERT(false);
  };

  for (size_t i = 0; i < lights.size(); i++) {
    if (attenuations[i])
      SetAttenuationComponent(shader_, Utils::ArrayName("attenuations", i),
                              *attenuations[i]);

    SetLightComponent(shader_, Utils::ArrayName("lights", i), lights[i]);
    SetLightSpecificComponent(shader_, getArrayName(lights[i]), lights[i], i,
                              attenuations[i] ? i : -1);
  }

  shader_.setUniformValue("dirLightCount", dirLightsCount);
  shader_.setUniformValue("pointLightCount", pointLightsCount);
  shader_.setUniformValue("spotLightCount", spotLightsCount);
}

}  // namespace s21
