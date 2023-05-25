
#pragma once

#include "Components.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

class CameraSystem : public System {
 public:
  CameraSystem() = default;
  ~CameraSystem() = default;

  void Update();
  void Init(ECS_Controller *scene);

 private:
  ECS_Controller *scene_ = nullptr;

  // void UpdateLookAtVectors(Camera &camera);
  // void ProcessKeyboardInput(Camera &camera, Input&input);
  // void ProcessMouseInput(Camera &camera   , Input&input);
};
}  // namespace s21
