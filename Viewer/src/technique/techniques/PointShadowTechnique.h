#pragma once
#include "ITechnique.h"

namespace s21 {
class PointShadowTechnique : public ITechnique {
 public:
  PointShadowTechnique() { Init(); }
  ~PointShadowTechnique() = default;

  void Init() final;
};
}  // namespace s21
