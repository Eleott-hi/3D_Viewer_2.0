#pragma once
#include "i_technique.h"

namespace s21 {
class PointShadowTechnique : public ITechnique {
 public:
  PointShadowTechnique() { Init(); }
  ~PointShadowTechnique() = default;

  void Init() final {
    GenerateShaders(":/shaders/point_shadows_depth.vs",
                    ":/shaders/point_shadows_depth.fs",
                    ":/shaders/point_shadows_depth.gs");
  }
  void SetTexture(Texture const &texture) final {}
  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final {}
};
}  // namespace s21
