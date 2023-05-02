#pragma once

#include "i_technique.h"

namespace s21 {

class LightColorTechnique : public ITechnique {
 public:
  LightColorTechnique() { init(); }
  ~LightColorTechnique() {}

  virtual void init() override;

  virtual void setLight(
      QVector<std::tuple<Light *, BaseLightType *, Attenuation *>> lights)
      override;

  virtual void setColor(QColor c) override;

  virtual void setMaterial(Material const &material) override;

  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                      QMatrix4x4 model) override;
};

}  // namespace s21
