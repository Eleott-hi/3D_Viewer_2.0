#pragma once

#include "ITechnique.h"

namespace s21 {

class ShadowMappingTechnique : public ITechnique {
 public:
  ShadowMappingTechnique() { Init(); }
  ~ShadowMappingTechnique() {}

  void Init() final;
  void OnMVPLoaded() final;
  void SetTexture(Texture const &texture) final;
};

}  // namespace s21
