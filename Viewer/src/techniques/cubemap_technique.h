#pragma once

#include "i_technique.h"

namespace s21 {

class CubemapTechnique : public ITechnique {
 public:
  CubemapTechnique() { init(); }
  ~CubemapTechnique() {}

  virtual void init() override;
  void setTexture(Texture const &texture) override;
  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                      QMatrix4x4 model) override;
};

}  // namespace s21
