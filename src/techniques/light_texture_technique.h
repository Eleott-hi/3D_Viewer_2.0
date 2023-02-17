#pragma once

#include <QOpenGLExtraFunctions>
#include <string>

#include "i_technique.h"

namespace s21 {

class LightTextureTechnique : public ITechnique,
                              protected QOpenGLExtraFunctions {
 public:
  LightTextureTechnique();

  ~LightTextureTechnique() = default;

  virtual void init() override;

  virtual void ApplyLightSettings(
      std::vector<TransformComponent> const &transforms,
      std::vector<LightSettingsComponent> const &settings) override;

  virtual void setShininess(float shininess) override;

  virtual void setTexture(TextureComponent const &texture) override;

  virtual void setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                      const QMatrix4x4 &model) override;
};

}  // namespace s21
