#pragma once

#include "i_technique.h"

namespace s21 {

class PhysicalBasedRenderingTechnique : public ITechnique {
 public:
  PhysicalBasedRenderingTechnique() { init(); }

  ~PhysicalBasedRenderingTechnique() = default;

  void init() final;
  void Clear() final { index_ = 0; }
  void setMaterial(Material const &material) final;
  void setTexture(Texture const &texture) final;
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
  void setLight(QVector<Light> lights,
                QVector<std::optional<Attenuation>> attenuations) final;

 private:
  uint32_t index_ = 0;

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
