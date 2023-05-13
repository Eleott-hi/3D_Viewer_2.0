#pragma once

#include "i_technique.h"

namespace s21 {

class QuadTechnique : public ITechnique {
 public:
  QuadTechnique() { init(); }
  ~QuadTechnique() {}

  void init() final;
  void setTexture(Texture const &texture) final;

 private:
  int index_ = 0;
};

}  // namespace s21
