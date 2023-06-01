#include "PointShadowTechnique.h"
namespace s21 {

void PointShadowTechnique::Init() {
  GenerateShaders(":/shaders/point_shadows_depth.vs",
                  ":/shaders/point_shadows_depth.fs",
                  ":/shaders/point_shadows_depth.gs");
}

}  // namespace s21
