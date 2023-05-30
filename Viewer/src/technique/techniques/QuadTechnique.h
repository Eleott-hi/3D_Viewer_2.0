#pragma once

#include "i_technique.h"

namespace s21 {

class QuadTechnique : public ITechnique {
 public:
  QuadTechnique() { init(); }
  ~QuadTechnique() {}

  void init() final;
  void setTexture(Texture const &texture) final;
};

}  // namespace s21
