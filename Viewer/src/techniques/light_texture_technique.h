#ifndef SRC_HEADERS_TEXTURE_TECHNIQUE_H
#define SRC_HEADERS_TEXTURE_TECHNIQUE_H

#include <string>

#include "i_technique.h"

namespace s21 {

class LightTextureTechnique : public ITechnique {
 public:
  LightTextureTechnique() { init(); }

  ~LightTextureTechnique() = default;

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

#endif  // SRC_HEADERS_TEXTURE_TECHNIQUE_H
