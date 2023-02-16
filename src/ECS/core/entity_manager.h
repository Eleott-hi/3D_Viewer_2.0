#pragma once
#include <map>
#include <vector>

#include "types.h"

namespace s21 {
class EntityManager {
 public:
  // TODO(pintoved): optimization
  EntityID GetEntityId() {
    static EntityID entityCounter = 0;
    return entityCounter++;
  }

  EntityManager() = default;
  ~EntityManager() = default;

  EntityID NewEntity() {
    EntityID id = GetEntityId();
    entities_.insert({id, ComponentMask()});
    return id;
  }

  template <typename T>
  void AddComponent(EntityID id) {
    assert(entities_.find(id) != entities_.end() &&
           "EntityManager::AddComponent: No such entity");

    ComponentID componentId = GetComponentID<T>();
    entities_.at(id).set(componentId);
  }

  template <typename T>
  void RemoveComponent(EntityID id) {
    assert(entities_.find(id) != entities_.end() &&
           "EntityManager::RemoveComponent: No such entity");

    ComponentID componentId = GetComponentID<T>();
    entities_.at(id).set(componentId, false);
  }

  void DestroyEntity(EntityID id) {
    assert(entities_.find(id) != entities_.end() &&
           "EntityManager::DestroyEntity: No such entity");

    entities_.erase(id);
  }

  template <typename... ComponentTypes>
  bool EntityHasComponent(EntityID id) {
    assert(entities_.find(id) != entities_.end() &&
           "EntityManager::EntityHasComponent: No such entity");

    assert(sizeof...(ComponentTypes) > 0 &&
           "EntityManager::EntityHasComponent: ComponentTypes can not be zero");

    ComponentMask mask;
    ComponentID componentIDs[] = {GetComponentID<ComponentTypes>()...};
    for (int i = 0; i < sizeof...(ComponentTypes); i++)
      mask.set(componentIDs[i]);

    auto& componentMask = entities_.at(id);

    return mask == (mask & componentMask);
  }

  template <typename... ComponentTypes>
  std::vector<EntityID> GetEntities() {
    ComponentMask mask;
    std::vector<EntityID> tmp;
    tmp.reserve(entities_.size());

    ComponentID componentIDs[] = {GetComponentID<ComponentTypes>()...};

    for (int i = 0; i < sizeof...(ComponentTypes); i++)
      mask.set(componentIDs[i]);

    tmp = GetEntities(mask);

    return tmp;
  }

  std::vector<EntityID> GetEntities(ComponentMask mask) {
    std::vector<EntityID> tmp;
    tmp.reserve(entities_.size());

    for (auto& [entityID, componentMask] : entities_) {
      if (mask == (mask & componentMask)) tmp.push_back(entityID);
    }
    return tmp;
  }

  ComponentMask GetMask(EntityID id) {
    assert(entities_.find(id) != entities_.end() &&
           "EntityManager::GetMask: No such entity");

    return entities_.at(id);
  }

 private:
  std::map<EntityID, ComponentMask> entities_;
};
}  // namespace s21