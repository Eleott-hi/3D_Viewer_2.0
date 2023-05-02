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

  virtual void init() override;
  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                      QMatrix4x4 model) override;
  virtual void SetObjectID(int ObjectID) override;
};

}  // namespace s21
