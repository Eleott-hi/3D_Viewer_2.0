#pragma once

#include "i_technique.h"

namespace s21 {

class SimpleColorTechnique : public ITechnique {
 public:
  SimpleColorTechnique() { init(); }
  ~SimpleColorTechnique() = default;

  void init() final;
  void setMaterial(Material const& material) final;
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
