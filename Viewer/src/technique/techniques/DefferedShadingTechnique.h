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
  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
