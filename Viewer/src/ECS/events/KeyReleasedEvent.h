#pragma once

#include <QKeyEvent>

#include "core/Event.h"

namespace s21 {
class KeyReleasedEvent : public Event {
 public:
  KeyReleasedEvent() = delete;
  KeyReleasedEvent(QKeyEvent* key_event) : key_event_(key_event) {}
  ~KeyReleasedEvent() = default;

  EventType GetType() override { return EventType::KeyReleased; }
  QKeyEvent* Key() const { return key_event_; }

 private:
  QKeyEvent* key_event_;
};
}  // namespace s21
