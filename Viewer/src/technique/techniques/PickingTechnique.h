// ================================================
// ===         Copyright 2022 pintoved          ===
// ================================================
#pragma once

#include "i_technique.h"

namespace s21 {

class PickingTechnique : public ITechnique {
 public:
  PickingTechnique() { init(); }
  ~PickingTechnique() = default;

  void init() final;
  void SetObjectID(int ObjectID) final;
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
