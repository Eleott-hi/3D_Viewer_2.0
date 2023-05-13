#pragma once

#include "i_technique.h"

namespace s21 {

class QuadTechnique : public ITechnique {
 public:
  QuadTechnique() { init(); }
  ~QuadTechnique() {}

  void Clear() final { index_ = 0; }
  void init() final;
  void setTexture(Texture const &texture) final;

 private:
  uint32_t index_ = 0;
};

}  // namespace s21
