#pragma once

#include "ITechnique.h"

namespace s21 {

class StencilOutlineTechnique : public ITechnique {
 public:
  StencilOutlineTechnique() { Init(); }
  ~StencilOutlineTechnique() = default;

  void Init() final;
  void OnMVPLoaded() final;
};

}  // namespace s21
