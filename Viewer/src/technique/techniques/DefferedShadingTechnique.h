#pragma once

#include "i_technique.h"

namespace s21 {

class DefferedShadingTechnique : public ITechnique {
 public:
  DefferedShadingTechnique() { init(); }
  ~DefferedShadingTechnique() {}

  void init() final;
  void setTexture(Texture const &texture);
  void setMaterial(Material const &material) final;
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
