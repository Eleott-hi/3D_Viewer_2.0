#pragma once

#include "ITechnique.h"

namespace s21 {

class SimpleColorTechnique : public ITechnique {
 public:
  SimpleColorTechnique() { Init(); }
  ~SimpleColorTechnique() = default;

  void Init() final;
  void OnMVPLoaded() final;
  void SetMaterial(Material const& material) final;
};

}  // namespace s21
