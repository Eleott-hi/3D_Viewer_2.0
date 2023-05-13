#pragma once

#include "i_technique.h"

namespace s21 {

class DefferedShadingTechnique : public ITechnique {
 public:
  DefferedShadingTechnique() { init(); }
  ~DefferedShadingTechnique() {}

  virtual void init() override;
  void setTexture(Texture const &texture);
  virtual void setMaterial(Material const &material) override;
  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                      QMatrix4x4 model) override;

 private:
  uint32_t texture_index_ = 0;
};

}  // namespace s21
