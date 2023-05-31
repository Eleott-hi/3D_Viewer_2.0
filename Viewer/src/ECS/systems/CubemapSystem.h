
#pragma once

#include <QOpenGLExtraFunctions>

#include "Components.h"
#include "TechniqueStrategy.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

class CubemapSystem : public System, protected QOpenGLExtraFunctions {
 public:
  CubemapSystem();
  ~CubemapSystem() = default;

  void Update();
  void Init(ECS_Controller *scene, TechniqueStrategy *technique);

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
  Texture cubemapTexture_;

  uint32_t loadCubemap(std::vector<std::string> faces);
  void RenderCube();
  void irradiance_convolution();
};
}  // namespace s21
