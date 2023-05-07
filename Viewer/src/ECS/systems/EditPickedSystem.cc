#include "EditPickedSystem.h"

namespace s21 {

void EditPickedSystem::Init(ECS_Controller *scene) { scene_ = scene; }

void EditPickedSystem::Update() {
  for (auto entity : entities_) {
    if (scene_->EntityHasComponent<Model>(entity)) qDebug() << "Model";
    if (scene_->EntityHasComponent<Material>(entity)) qDebug() << "Material";
    if (scene_->EntityHasComponent<Transform>(entity)) qDebug() << "Transform";
    if (scene_->EntityHasComponent<Light>(entity)) qDebug() << "Light";
    if (scene_->EntityHasComponent<Texture>(entity)) qDebug() << "Texture";
  }
}

}  // namespace s21
