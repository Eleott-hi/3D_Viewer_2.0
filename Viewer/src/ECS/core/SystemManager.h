#pragma once

#include <map>
#include <memory>

#include "System.h"
#include "types.h"

namespace s21 {
class SystemManager {
 public:
  SystemManager() = default;
  ~SystemManager() = default;

  template <typename T>
  static SystemID GetSystemID() {
    return typeid(T).hash_code();
  }

  template <typename T>
  std::shared_ptr<T> RegisterSystem() {
    SystemID id = GetSystemID<T>();

    assert(systems_.find(id) == systems_.end() &&
           "SystemManager::RegisterSystem: System already registered");

    auto system = std::make_shared<T>();
    systems_.insert({id, system});
    masks_.insert({id, ComponentMask()});

    return system;
  }

  template <typename T>
  void ChangeSystemMask(ComponentMask mask) {
    SystemID id = GetSystemID<T>();
    assert(systems_.find(id) != systems_.end() &&
           "SystemManager::AddComponent: System is not registered");

    masks_.at(id) = mask;
  }

  void EntityDestroyed(EntityID id) {
    for (auto &[systemID, system] : systems_) system->entities_.erase(id);
  }

  void EntityMaskChanged(EntityID id, ComponentMask mask) {
    for (auto &[systemID, system] : systems_) {
      ComponentMask systemMask = masks_.at(systemID);
      if ((systemMask & mask) == systemMask) {
        system->entities_.insert(id);
      } else {
        system->entities_.erase(id);
      }
    }
  }

 private:
  std::map<SystemID, std::shared_ptr<System>> systems_;
  std::map<SystemID, ComponentMask> masks_;
};
}  // namespace s21
