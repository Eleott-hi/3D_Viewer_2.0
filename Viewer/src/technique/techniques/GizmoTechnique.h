#pragma once

#include "ITechnique.h"

namespace s21 {

class GizmoTechnique : public ITechnique {
 public:
  GizmoTechnique() { Init(); }
  ~GizmoTechnique() {}

  void Init() final;
  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
