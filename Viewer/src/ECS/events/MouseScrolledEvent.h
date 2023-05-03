#pragma once

#include <QKeyEvent>

#include "core/Event.h"

namespace s21 {
class MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent() = delete;
  MouseScrolledEvent(float scroll) : scroll_(scroll) {}
  ~MouseScrolledEvent() = default;

  EventType GetType() override { return EventType::MouseScrolled; }
  float Scroll() const { return scroll_; }

 private:
  float scroll_;
};
}  // namespace s21
