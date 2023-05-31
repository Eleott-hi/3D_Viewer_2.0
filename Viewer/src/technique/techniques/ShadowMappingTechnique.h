#pragma once

#include "ITechnique.h"

namespace s21 {

class ShadowMappingTechnique : public ITechnique {
 public:
  ShadowMappingTechnique() { Init(); }
  ~ShadowMappingTechnique() {}

  void Init() final;
  void SetTexture(Texture const &texture) final;

  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
