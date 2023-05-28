#pragma once

#include "i_technique.h"

namespace s21 {

class GizmoTechnique : public ITechnique {
 public:
  GizmoTechnique() { init(); }
  ~GizmoTechnique() {}

  void init() final;
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;

 private:
  uint32_t index_ = 0;
};

}  // namespace s21
