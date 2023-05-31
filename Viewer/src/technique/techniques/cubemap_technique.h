#pragma once

#include "i_technique.h"

namespace s21 {

class CubemapTechnique : public ITechnique {
 public:
  CubemapTechnique() { Init(); }
  ~CubemapTechnique() {}

  void Init() final;
  void SetTexture(Texture const &texture) final;
  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
