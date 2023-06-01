// ================================================
// ===         Copyright 2022 pintoved          ===
// ================================================
#pragma once

#include "ITechnique.h"

namespace s21 {

class PickingTechnique : public ITechnique {
 public:
  PickingTechnique() { Init(); }
  ~PickingTechnique() = default;

  void Init() final;
  void SetObjectID(int ObjectID) final;
  void OnMVPLoaded() final;
};

}  // namespace s21
