#pragma once

#include "i_technique.h"

namespace s21 {

class SimpleColorTechnique : public ITechnique {
 public:
  SimpleColorTechnique() { Init(); }
  ~SimpleColorTechnique() = default;

  void Init() final;
  void SetMaterial(Material const& material) final;
  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
