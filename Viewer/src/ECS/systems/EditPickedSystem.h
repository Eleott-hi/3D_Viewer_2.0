
#pragma once

#include <optional>

#include "Components.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

enum class EditType { Transform, Model, Material, Light, Texture };

class EditPickedSystem : public System {
 public:
  EditPickedSystem() {}
  ~EditPickedSystem() {}

  void Init(ECS_Controller *scene);
  void Update();

  template <typename Type>
  void UpdateComponent(Type const &component) {
    for (auto &entity : entities_)
      if (scene_->EntityHasComponent<Type>(entity)) {
        auto &component_ = scene_->GetComponent<Type>(entity);
        component_ = component;
        qDebug() << "UpdateComponent" << component.translation.x();
      }
  }

  template <typename Type>
  std::vector<Type *> GetComponents() {
    std::vector<Type *> components;

    for (auto &entity : entities_)
      if (scene_->EntityHasComponent<Type>(entity))
        components.push_back(&scene_->GetComponent<Type>(entity));

    return components;
  }

 private:
  ECS_Controller *scene_;
  // EntityID edited_entity_ = -1;
};

}  // namespace s21