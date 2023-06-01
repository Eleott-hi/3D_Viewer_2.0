#pragma once

#include "ITechnique.h"

namespace s21 {

class QuadTechnique : public ITechnique {
 public:
  QuadTechnique() { Init(); }
  ~QuadTechnique() = default;

  void Init() final;
  void SetTexture(Texture const &texture) final;
};

}  // namespace s21
