
#pragma once

#include <QOpenGLExtraFunctions>

#include "Components.h"
#include "core/ECS_Controller.h"
#include "core/System.h"

namespace s21 {

class InputSystem : public System, protected QOpenGLExtraFunctions {
 public:
  InputSystem() { initializeOpenGLFunctions(); }
  ~InputSystem() = default;

  void Init(ECS_Controller *scene) { scene_ = scene; }
  void Update();

 private:
  ECS_Controller *scene_;

  void ProcessKeyPressed(Input &input);
  void ProcessMouseMovement(Input &input, bool constrainPitch = true);
};

}  // namespace s21
