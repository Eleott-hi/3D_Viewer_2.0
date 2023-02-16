#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>

namespace s21 {
typedef size_t EntityID;
typedef size_t ComponentID;
typedef size_t SystemID;
typedef size_t EventID;
typedef size_t w;

const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 5;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

ComponentID GetComponentID();

template <class T>
ComponentID GetComponentID() {
  static ComponentID componentId = GetComponentID();
  return componentId;
}

#define BIND_EVENT_FN(fn)                            \
  [this](auto&&... args) -> void {                   \
    this->fn(std::forward<decltype(args)>(args)...); \
  }

}  // namespace s21