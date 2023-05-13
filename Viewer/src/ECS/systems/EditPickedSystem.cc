#include "EditPickedSystem.h"

namespace s21 {

void EditPickedSystem::Init(ECS_Controller *scene) { scene_ = scene; }

void EditPickedSystem::Update() {
  auto picked_entities = scene_->GetEntities<PickingTag>();

  for (auto entity : entities_) {
  }
}

}  // namespace s21
