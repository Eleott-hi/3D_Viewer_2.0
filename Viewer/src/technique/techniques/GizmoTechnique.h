#pragma once

#include "ITechnique.h"

namespace s21 {

class GizmoTechnique : public ITechnique {
 public:
  GizmoTechnique() { Init(); }
  ~GizmoTechnique() {}

  void Init() final;
  void OnMVPLoaded() final;
};

}  // namespace s21
