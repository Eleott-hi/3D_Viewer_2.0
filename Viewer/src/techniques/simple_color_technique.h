#pragma once

#include "i_technique.h"

namespace s21 {

class SimpleColorTechnique : public ITechnique {
 public:
  SimpleColorTechnique() { init(); }
  ~SimpleColorTechnique() {}

  virtual void init() override;

  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                      QMatrix4x4 model) override;

  virtual void setColor(QColor c) override;
};

}  // namespace s21
