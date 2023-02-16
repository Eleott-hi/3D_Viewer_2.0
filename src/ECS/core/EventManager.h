#pragma once

#include <functional>
#include <map>
#include <vector>

#include "Event.h"

namespace s21 {

class EventManager {
 public:
  EventManager() = default;
  ~EventManager() = default;

  void AddListener(EventType type,
                   std::function<void(Event &)> const &listener) {
    listeners_[type].push_back(listener);
  }

  void Depatch(Event &event) {
    EventType type = event.GetType();

    for (auto &listener : listeners_[type]) listener(event);
  }

 private:
  std::map<EventType, std::vector<std::function<void(Event &)>>> listeners_;
};

}  // namespace s21
