#pragma once

#include <QVector>
#include <functional>
#include <unordered_map>

#include "Components.h"
#include "core/ECS_Controller.h"

namespace s21 {

class Hierarchy {
 public:
  static void AddChildren(ECS_Controller* scene, EntityID parent,
                          QVector<EntityID> children) {
    for (auto child : children) SetParent(scene, parent, child);
  }

  static void SetParent(ECS_Controller* scene, EntityID parent,
                        EntityID child) {
    Q_ASSERT(child != parent);

    CheckHierarchyComponent(scene, child);
    CheckHierarchyComponent(scene, parent);

    {
      auto& hierarchy = scene->GetComponent<HierarchyComponent>(child);
      hierarchy.parent = parent;
    }

    {
      auto& hierarchy = scene->GetComponent<HierarchyComponent>(parent);
      hierarchy.children << child;
    }
  }

  static void ClearMatricies() { matricies_.clear(); }

  static QMatrix4x4 CalcInheriteMatrix(ECS_Controller* scene, EntityID entity) {
    if (matricies_.find(entity) == matricies_.end() &&
        scene->EntityHasComponent<Transform>(entity)) {
      auto& transform = scene->GetComponent<Transform>(entity);
      auto& hierarchy = scene->GetComponent<HierarchyComponent>(entity);

      matricies_[entity] = CalcInheriteMatrix(scene, hierarchy.parent) *
                           transform.GetModelMatrix();
    }

    return matricies_[entity];
  }

  void HeirarchyFunction(std::function<void(ECS_Controller*, EntityID)> func) {}

 private:
  static std::unordered_map<int, QMatrix4x4> matricies_;

  static void CheckHierarchyComponent(ECS_Controller* scene, EntityID entity) {
    if (!scene->EntityHasComponent<HierarchyComponent>(entity))
      scene->AddComponent<HierarchyComponent>(entity);
  }
};

}  // namespace s21
