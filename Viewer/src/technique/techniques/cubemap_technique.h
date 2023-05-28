#pragma once

#include "i_technique.h"

namespace s21 {

class CubemapTechnique : public ITechnique {
 public:
  CubemapTechnique() { init(); }
  ~CubemapTechnique() {}

  void init() final;
  void setTexture(Texture const &texture) final;
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
