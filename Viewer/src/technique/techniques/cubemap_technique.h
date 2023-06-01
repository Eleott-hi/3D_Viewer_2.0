#pragma once

#include "ITechnique.h"

namespace s21 {

class CubemapTechnique : public ITechnique {
 public:
  CubemapTechnique() { Init(); }
  ~CubemapTechnique() {}

  void Init() final;
  void SetTexture(Texture const &texture) final;
  void OnMVPLoaded() final;
};

}  // namespace s21
