#pragma once

#include <QKeyEvent>

#include "core/Event.h"

namespace s21 {
class KeyPressedEvent : public Event {
 public:
  KeyPressedEvent() = delete;
  KeyPressedEvent(QKeyEvent* key_event) : key_event_(key_event) {}
  ~KeyPressedEvent() = default;

  EventType GetType() override { return EventType::KeyPressed; }
  QKeyEvent* Key() const { return key_event_; }

 private:
  QKeyEvent* key_event_;
};
}  // namespace s21
