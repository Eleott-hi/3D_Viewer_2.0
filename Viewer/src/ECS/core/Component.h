#pragma once
#include <map>
#include <memory>

#include "types.h"

namespace s21 {
class IComponentPool {
 public:
  virtual ~IComponentPool() = default;
  virtual void EntityDestroyed(EntityID entityID) = 0;
};

template <class T>
class ComponentPool : public IComponentPool {
 public:
  ComponentPool() = default;
  ~ComponentPool() = default;

  void AddComponent(EntityID entityID, T component) {
    assert(
        data_.find(entityID) == data_.end() &&
        "ComponentPool::AddComponent: Component already exist in this entity");

    data_.insert({entityID, component});
  }

  void RemoveComponent(EntityID entityID) {  //
    assert(data_.find(entityID) != data_.end() &&
           "ComponentPool::RemoveComponent: No such component in this entity");

    data_.erase(entityID);
  }

  T& GetComponent(EntityID entityID) {  //
    assert(data_.find(entityID) != data_.end() &&
           "ComponentPool::GetComponent: No such component in this entity");

    return data_.at(entityID);
  }

  void EntityDestroyed(EntityID entityID) override {
    if (data_.find(entityID) != data_.end())  //
      RemoveComponent(entityID);
  }

 private:
  std::map<EntityID, T> data_;
};
}  // namespace s21