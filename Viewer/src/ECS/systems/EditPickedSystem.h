
#pragma once

#include "Components.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

class EditPickedSystem : public System {
 public:
  EditPickedSystem() {}
  ~EditPickedSystem() {}

  void Init(ECS_Controller *scene);
  void Update();

 private:
  ECS_Controller *scene_;
};

}  // namespace s21