#pragma once
#include "i_technique.h"

namespace s21 {
class PointShadowTechnique : public ITechnique {
 public:
  PointShadowTechnique() { init(); }
  ~PointShadowTechnique() = default;

  void init() final {
    GenerateShaders(":/shaders/point_shadows_depth.vs",
                    ":/shaders/point_shadows_depth.fs",
                    ":/shaders/point_shadows_depth.gs");
  }
  void setTexture(Texture const &texture) final {}
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final {}

 private:
  uint32_t index_ = 0;
};
}  // namespace s21
