#pragma once

#include <string>

#include "i_technique.h"

namespace s21 {

class NormalMapTechnique : public ITechnique {
 public:
  NormalMapTechnique() { init(); }

  ~NormalMapTechnique() = default;

  void init() final;
  void Clear() final { index_ = 0; }
  void setMaterial(Material const &material) final;
  void setTexture(Texture const &texture) final;
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
  void setLight(QVector<std::tuple<Light *, BaseLightType *, Attenuation *>>
                    lights) final;

 private:
  uint32_t index_ = 0;
};

}  // namespace s21
