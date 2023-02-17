#pragma once

#include "EventManager.h"
#include "SystemManager.h"
#include "component_manager.h"
#include "entity_manager.h"

namespace s21 {
class ECS_Controller {
 public:
  ECS_Controller() {
    componentManager = std::make_unique<ComponentManager>();
    entityManager = std::make_unique<EntityManager>();
    systemManager = std::make_unique<SystemManager>();
    eventManager = std::make_unique<EventManager>();
  }

  ~ECS_Controller() = default;

  // ================== Entity methods ==================
  EntityID NewEntity() {  //
    return entityManager->NewEntity();
  }

  void DestroyEntity(EntityID id) {
    entityManager->DestroyEntity(id);
    componentManager->EntityDestroyed(id);
    systemManager->EntityDestroyed(id);
  }

  // TODO (pintoved): check ComponentTypes for registration;
  template <typename... ComponentTypes>
  std::vector<EntityID> GetEntities() {
    return entityManager->GetEntities<ComponentTypes...>();
  }

  template <typename... ComponentTypes>
  bool EntityHasComponent(EntityID id) {
    return entityManager->EntityHasComponent<ComponentTypes...>(id);
  }

  // ================== Component methods ==================
  template <typename T>
  void RegisterComponent() {
    componentManager->RegisterComponent<T>();
  }

  template <typename T>
  void AddComponent(EntityID id, T component = {}) {
    componentManager->AddComponent<T>(id, component);
    entityManager->AddComponent<T>(id);

    ComponentMask entityMask = entityManager->GetMask(id);
    systemManager->EntityMaskChanged(id, entityMask);
  }

  template <typename T>
  void RemoveComponent(EntityID id) {
    componentManager->RemoveComponent<T>(id);
    entityManager->RemoveComponent<T>(id);

    ComponentMask entityMask = entityManager->GetMask(id);
    systemManager->EntityMaskChanged(id, entityMask);
  }

  template <typename T>
  T& GetComponent(EntityID id) {
    return componentManager->GetComponent<T>(id);
  }

  // ================== System methods ==================
  template <typename T>
  std::shared_ptr<T> RegisterSystem() {
    return systemManager->RegisterSystem<T>();
  }

  template <typename T>
  void ChangeSystemMask(ComponentMask mask) {
    systemManager->ChangeSystemMask<T>(mask);
  }

  // ================== Event methods ==================
  void AddEventListener(EventType type,
                        std::function<void(Event&)> const& listener) {
    eventManager->AddListener(type, listener);
  }

  void Despatch(Event& event) {  //
    eventManager->Despatch(event);
  }

 private:
  std::unique_ptr<ComponentManager> componentManager;
  std::unique_ptr<EntityManager> entityManager;
  std::unique_ptr<SystemManager> systemManager;
  std::unique_ptr<EventManager> eventManager;
};
}  // namespace s21
