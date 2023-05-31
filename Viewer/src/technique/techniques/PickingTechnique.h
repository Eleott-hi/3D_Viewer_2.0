// ================================================
// ===         Copyright 2022 pintoved          ===
// ================================================
#pragma once

#include "i_technique.h"

namespace s21 {

class PickingTechnique : public ITechnique {
 public:
  PickingTechnique() { Init(); }
  ~PickingTechnique() = default;

  void Init() final;
  void SetObjectID(int ObjectID) final;
  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21
