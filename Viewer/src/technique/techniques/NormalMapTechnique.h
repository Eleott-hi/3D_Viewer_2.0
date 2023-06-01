#pragma once

#include "ITechnique.h"

namespace s21 {

class NormalMapTechnique : public ITechnique {
 public:
  NormalMapTechnique() { Init(); }

  ~NormalMapTechnique() = default;

  void Init() final;
  void SetMaterial(Material const &material) final;
  void SetTexture(Texture const &texture) final;
  void OnMVPLoaded() final;
  void SetLight(QVector<Light> lights,
                QVector<std::optional<Attenuation>> attenuations) final;

 private:
  void SetLightComponent(ShaderProgram &shader, std::string const &type,
                         Light const &light);
  void SetAttenuationComponent(ShaderProgram &shader, std::string const &type,
                               Attenuation const &attenuation);
  void SetLightSpecificComponent(ShaderProgram &shader, std::string const &type,
                                 Light const &light, int light_index,
                                 int attenuation_index);
};

}  // namespace s21
