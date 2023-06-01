#pragma once

#include "ITechnique.h"

namespace s21 {
class PointShadowRenderTechnique : public ITechnique {
 public:
  PointShadowRenderTechnique() { Init(); }
  ~PointShadowRenderTechnique() = default;

  void Init() final;
  void OnMVPLoaded() final;
  void SetTexture(Texture const &texture) final;
  void SetMaterial(Material const &material) final;
};
}  // namespace s21
