#include "light_texture_technique.h"

namespace s21 {

LightTextureTechnique::LightTextureTechnique() {
  initializeOpenGLFunctions();
  init();
}

void LightTextureTechnique::init() {
  GenerateShaders("shaders/light_texture_shader_v.c",
                  "shaders/light_texture_shader_f.c");
}

void LightTextureTechnique::ApplyLightSettings(
    std::vector<TransformComponent> const &transforms,
    std::vector<LightSettingsComponent> const &settings) {
  shader_.setUniformValue("u_pointLightsCount", (int)transforms.size());

  for (size_t i = 0; i < transforms.size(); i++) {
    auto const &position = transforms.at(i).position;
    auto const &[ambient, diffuse, specular, constants] = settings.at(i);

    std::string pointLights = "u_pointLights[" + std::to_string(i) + "].";

    shader_.setUniformValue((pointLights + "position").c_str(), position);
    shader_.setUniformValue((pointLights + "ambient").c_str(), ambient);
    shader_.setUniformValue((pointLights + "diffuse").c_str(), diffuse);
    shader_.setUniformValue((pointLights + "specular").c_str(), specular);
    shader_.setUniformValue((pointLights + "constant").c_str(), constants.x());
    shader_.setUniformValue((pointLights + "linear").c_str(), constants.y());
  }
}

void LightTextureTechnique::setShininess(float shininess) {
  shader_.setUniformValue("u_material.shininess", shininess);
}

void LightTextureTechnique::setTexture(TextureComponent const &texture) {
  auto const &[diffuse, specular, normal, height] = texture;

  glActiveTexture(GL_TEXTURE0 + 0);
  shader_.setUniformValue("u_material.diffuse", 0);
  glBindTexture(GL_TEXTURE_2D, diffuse);

  glActiveTexture(GL_TEXTURE0 + 1);
  shader_.setUniformValue("u_material.specular", 1);
  glBindTexture(GL_TEXTURE_2D, diffuse);
}

void LightTextureTechnique::setMVP(const QMatrix4x4 &proj,
                                   const QMatrix4x4 &view,
                                   const QMatrix4x4 &model) {
  shader_.setUniformValue("u_Model", model);
  shader_.setUniformValue("u_View", view);
  shader_.setUniformValue("u_Projection", proj);

  auto tmp = view.inverted();
  shader_.setUniformValue("u_viewPos",
                          QVector3D{tmp(0, 3), tmp(1, 3), tmp(2, 3)});
}

}  // namespace s21
