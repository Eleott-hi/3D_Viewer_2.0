#pragma once

#include <set>

#include "types.h"

namespace s21 {

class System {
 public:
  System() = default;
  virtual ~System() = default;

  std::set<EntityID> entities_;
};
}  // namespace s21