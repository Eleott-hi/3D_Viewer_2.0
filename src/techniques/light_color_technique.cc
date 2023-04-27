#include "light_color_technique.h"

#include <string>

namespace s21 {

void LightColorTechnique::init() {
  GenerateShaders("shaders/light_color_shader_v.c",
                  "shaders/light_color_shader_f.c");
}

void SetLightComponent(QOpenGLShaderProgram &shader,
                       LightComponent const &light,
                       std::string const &light_type) {
  auto const &[ambient, diffuse, specular, constants] = light;

  shader.setUniformValue((light_type + ".ambient").c_str(), ambient);
  shader.setUniformValue((light_type + ".diffuse").c_str(), diffuse);
  shader.setUniformValue((light_type + ".specular").c_str(), specular);
  shader.setUniformValue((light_type + ".constant").c_str(), constants.x());
  shader.setUniformValue((light_type + ".linear").c_str(), constants.y());
}

void LightColorTechnique::ApplyLightSettings(
    std::vector<TransformComponent> const &transforms,
    std::vector<LightComponent> const &light) {
  shader_.setUniformValue("u_pointLightsCount", (int)transforms.size());

  shader_.setUniformValue("u_dirLightsCount", 1);
  shader_.setUniformValue("u_dirLights[0].direction", QVector3D{0, 0, -1});
  shader_.setUniformValue("u_dirLights[0].ambient", QVector3D{ 0.5, 0.5, 0.5});
  shader_.setUniformValue("u_dirLights[0].diffuse", QVector3D{ 0.5, 0.5, 0.5});
  shader_.setUniformValue("u_dirLights[0].specular", QVector3D{0.5, 0.5, 0.5});


  for (size_t i = 0; i < transforms.size(); i++) {
    auto const &position = transforms.at(i).position;
    std::string pointLight = "u_pointLights[" + std::to_string(i) + "]";

    SetLightComponent(shader_, light.at(i), pointLight);
    shader_.setUniformValue((pointLight + ".position").c_str(), position);
  }
}

void LightColorTechnique::setColor(const QColor &c) {
  shader_.setUniformValue("u_Color", c);
}

void LightColorTechnique::setMaterial(MaterialComponent const &material) {
  shader_.setUniformValue("u_material.shininess", material.shininess);
}

void LightColorTechnique::setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                                 const QMatrix4x4 &model) {
  shader_.setUniformValue("u_Model", model);
  shader_.setUniformValue("u_View", view);
  shader_.setUniformValue("u_Projection", proj);

  auto tmp = view.inverted();
  shader_.setUniformValue("u_viewPos",
                          QVector3D{tmp(0, 3), tmp(1, 3), tmp(2, 3)});
}

}  // namespace s21
