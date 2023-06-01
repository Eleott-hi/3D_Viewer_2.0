#include "LightTextureTechnique.h"

#include "Utils.h"

namespace s21 {
void LightTextureTechnique::Init() {
  GenerateShaders(":/shaders/light_texture_shader.vs",
                  ":/shaders/light_texture_shader.fs");
}

void LightTextureTechnique::SetTexture(Texture const &texture) {
  shader_.setUniformValue(texture.type.c_str(), index_);
  glActiveTexture(GL_TEXTURE0 + index_);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  index_++;
}

void LightTextureTechnique::SetMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                   QMatrix4x4 model) {
  shader_.setUniformValue("u_Model", model);
  shader_.setUniformValue("u_View", view_);
  shader_.setUniformValue("u_Projection", projection_);

  shader_.setUniformValue("viewPos", QVector3D{view_.inverted().column(3)});
}

void LightTextureTechnique::SetMaterial(Material const &material) {
  SetTexture({material.diffuse.id, "material.diffuseMap"});
  SetTexture({material.specular.id, "material.specularMap"});
  shader_.setUniformValue("material.shininess", material.shininess);
}

void LightTextureTechnique::SetLightComponent(ShaderProgram &shader,
                                              std::string const &type,
                                              Light const &light) {
  shader.setUniformValue(Utils::StructName(type, "ambient").c_str(),
                         light.ambient);
  shader.setUniformValue(Utils::StructName(type, "diffuse").c_str(),
                         light.diffuse);
  shader.setUniformValue(Utils::StructName(type, "specular").c_str(),
                         light.specular);
}

void LightTextureTechnique::SetAttenuationComponent(
    ShaderProgram &shader, std::string const &type,
    Attenuation const &attenuation) {
  auto const &[constant, linear, quadratic] = attenuation;
  shader.setUniformValue(Utils::StructName(type, "linear").c_str(), linear);
  shader.setUniformValue(Utils::StructName(type, "constant").c_str(), constant);
  shader.setUniformValue(Utils::StructName(type, "quadratic").c_str(),
                         quadratic);
}

void LightTextureTechnique::SetLightSpecificComponent(ShaderProgram &shader,
                                                      std::string const &type,
                                                      Light const &light,
                                                      int light_index,
                                                      int attenuation_index) {
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

void LightTextureTechnique::SetLight(
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
