#include "light_color_technique.h"

#include <string>

namespace s21 {

// void SetLightComponent(QOpenGLShaderProgram &shader, Light *light, int index)
// {
//   auto const &[ambient, diffuse, specular] = *light;
//   std::string s = "u_lights[" + std::to_string(index) + "].";

//   shader.setUniformValue((s + "ambient").c_str(), ambient);
//   shader.setUniformValue((s + "diffuse").c_str(), diffuse);
//   shader.setUniformValue((s + "specular").c_str(), specular);
// }

// void SetAttenuation(QOpenGLShaderProgram &shader, Attenuation *attenuation,
//                     int index) {
//   auto const &[constant, linear, quadratic] = *attenuation;
//   std::string s = "u_attenuations[" + std::to_string(index) + "].";

//   shader.setUniformValue((s + "constant").c_str(), constant);
//   shader.setUniformValue((s + "linear").c_str(), linear);
//   shader.setUniformValue((s + "quadratic").c_str(), quadratic);
// }

// void SetIndecies(QOpenGLShaderProgram &shader, std::string const &type,
//                  int light_index, int attenuation_index) {
//   shader.setUniformValue((type + ".light_index").c_str(), light_index);
//   shader.setUniformValue((type + ".attenuation_index").c_str(),
//                          attenuation_index);
// }

// void SetSpotLight(QOpenGLShaderProgram &shader, SpotLight *light,
//                   std::string const &s) {
//   auto const &[position, direction, inner_cone, outer_cone] = *light;

//   shader.setUniformValue((s + ".position").c_str(), position);
//   shader.setUniformValue((s + ".direction").c_str(), direction);
//   shader.setUniformValue((s + ".inner_cone").c_str(),
//                          qCos(qDegreesToRadians(inner_cone)));
//   shader.setUniformValue((s + ".outer_cone").c_str(),
//                          qCos(qDegreesToRadians(outer_cone)));
// }

// void SetPointLight(QOpenGLShaderProgram &shader, PointLight *light,
//                    std::string const &s) {
//   auto const &[position] = *light;

//   shader.setUniformValue((s + ".position").c_str(), position);
// }

// void SetDirLight(QOpenGLShaderProgram &shader, DirectionalLight *light,
//                  std::string const &s) {
//   auto const &[direction] = *light;

//   shader.setUniformValue((s + ".direction").c_str(), direction);
// }

// void LightColorTechnique::setLight(
//     QVector<std::tuple<Light *, BaseLightType *, Attenuation *>> lights) {
//   int dirLightsCount = 0, pointLightsCount = 0,  //
//       spotLightsCount = 0, attenuationCount = 0;

//   for (size_t i = 0; i < lights.size(); i++) {
//     int attenuation_index = -1;

//     auto [light, base_light, attenuation] = lights[i];
//     SetLightComponent(shader_, light, i);

//     if (attenuation) {
//       SetAttenuation(shader_, attenuation, attenuationCount);
//       attenuation_index = attenuationCount++;
//     }

//     auto type = base_light->GetType();
//     std::string s_type;

//     switch (type) {
//       case LightType::DIRECTIONAL:
//         s_type = "u_dirLights[" + std::to_string(dirLightsCount++) + "]";
//         SetDirLight(shader_, (DirectionalLight *)base_light, s_type);
//         break;

//       case LightType::POINT:
//         s_type = "u_pointLights[" + std::to_string(pointLightsCount++) + "]";
//         SetPointLight(shader_, (PointLight *)base_light, s_type);
//         break;

//       case LightType::SPOT:
//         s_type = "u_spotLights[" + std::to_string(spotLightsCount++) + "]";
//         SetSpotLight(shader_, (SpotLight *)base_light, s_type);
//         break;

//       default:
//         break;
//     }

//     SetIndecies(shader_, s_type, i, attenuation_index);
//   }

//   shader_.setUniformValue("u_dirLightCount", dirLightsCount);
//   shader_.setUniformValue("u_pointLightCount", pointLightsCount);
//   shader_.setUniformValue("u_spotLightCount", spotLightsCount);
// }

void LightColorTechnique::init() {
  GenerateShaders(":/shaders/light_color_shader.vs",
                  ":/shaders/light_color_shader.fs");
}

void LightColorTechnique::setMaterial(Material const &material) {
  shader_.setUniformValue("u_Color", material.color);
  shader_.setUniformValue("u_material.shininess", material.shininess);
}

void LightColorTechnique::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                                 QMatrix4x4 model) {
  shader_.setUniformValue("u_Model", model);
  shader_.setUniformValue("u_View", view);
  shader_.setUniformValue("u_Projection", proj);

  auto tmp = view.inverted();
  shader_.setUniformValue("u_viewPos",
                          QVector3D{tmp(0, 3), tmp(1, 3), tmp(2, 3)});
}

}  // namespace s21
