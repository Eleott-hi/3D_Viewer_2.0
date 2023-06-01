
#pragma once

#include "Components.h"
#include "TechniqueStrategy.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

class CameraSystem : public System {
 public:
  CameraSystem() = default;
  ~CameraSystem() = default;

  void Update();
  void Init(ECS_Controller *scene, TechniqueStrategy *technique);

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
};
}  // namespace s21
