#pragma once

#include <map>

#include "component.h"
#include "types.h"

namespace s21 {

class ComponentManager {
 public:
  ComponentManager() = default;
  ~ComponentManager() = default;

  template <class T>
  void RegisterComponent() {
    ComponentID id = GetComponentID<T>();
    assert(componentPools_.find(id) == componentPools_.end() &&
           "ComponentManager::RegisterComponent: Component already registered");
    componentPools_.insert({id, std::make_shared<ComponentPool<T>>()});
  }

  template <class T>
  void AddComponent(EntityID entityID, T component) {
    GetComponentPool<T>()->AddComponent(entityID, component);
  }

  template <class T>
  void RemoveComponent(EntityID entityID) {
    GetComponentPool<T>()->RemoveComponent(entityID);
  }

  template <class T>
  T& GetComponent(EntityID entityID) {
    return GetComponentPool<T>()->GetComponent(entityID);
  }

  void EntityDestroyed(EntityID entityID) {
    for (auto& [componentID, componentPool] : componentPools_)
      componentPool->EntityDestroyed(entityID);
  }

 private:
  std::map<ComponentID, std::shared_ptr<IComponentPool>> componentPools_;

  template <class T>
  std::shared_ptr<ComponentPool<T>> GetComponentPool() {
    ComponentID id = GetComponentID<T>();

    assert(componentPools_.find(id) != componentPools_.end() &&
           "ComponentManager::GetComponentPool: Component is not registered");

    return std::static_pointer_cast<ComponentPool<T>>(componentPools_.at(id));
  }
};
}  // namespace s21