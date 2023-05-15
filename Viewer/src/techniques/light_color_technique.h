#pragma once

#include "i_technique.h"

namespace s21 {

class LightColorTechnique : public ITechnique {
 public:
  LightColorTechnique() { init(); }
  ~LightColorTechnique() {}

  virtual void init() override;

  virtual void setMaterial(Material const &material) override;

  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                      QMatrix4x4 model) override;

  void setLight(QVector<Light> lights,
                QVector<std::optional<Attenuation>> attenuations) final;

 private:
  void SetLightComponent(QOpenGLShaderProgram &shader, std::string const &type,
                         Light const &light);
  void SetAttenuationComponent(QOpenGLShaderProgram &shader,
                               std::string const &type,
                               Attenuation const &attenuation);
  void SetLightSpecificComponent(QOpenGLShaderProgram &shader,
                                 std::string const &type, Light const &light,
                                 int light_index, int attenuation_index);
};

}  // namespace s21
