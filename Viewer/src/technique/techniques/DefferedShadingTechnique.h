#pragma once

#include "ITechnique.h"

namespace s21 {

class DefferedShadingTechnique : public ITechnique {
 public:
  DefferedShadingTechnique() { Init(); }
  ~DefferedShadingTechnique() {}

  void Init() final;
  void SetTexture(Texture const &texture);
  void SetMaterial(Material const &material) final;
  void OnMVPLoaded() final;
};

}  // namespace s21
