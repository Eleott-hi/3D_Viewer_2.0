#pragma once

#include "i_technique.h"

namespace s21 {

class QuadTechnique : public ITechnique {
 public:
  QuadTechnique() { init(); }
  ~QuadTechnique() {}

  virtual void init() override;
  virtual void setTextureID(uint32_t id) override;
};

}  // namespace s21
