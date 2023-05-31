#pragma once

#include "i_technique.h"

namespace s21 {

class QuadTechnique : public ITechnique {
 public:
  QuadTechnique() { Init(); }
  ~QuadTechnique() {}

  void Init() final;
  void SetTexture(Texture const &texture) final;
};

}  // namespace s21
