#pragma once

#include <QPoint>

#include "core/Event.h"

namespace s21 {
class MouseMovedEvent : public Event {
 public:
  MouseMovedEvent() = delete;
  MouseMovedEvent(QPoint offset) : offset_(offset) {}
  ~MouseMovedEvent() = default;

  EventType GetType() override { return EventType::MouseMoved; }
  QPoint Offset() const { return offset_; }

 private:
  QPoint offset_;
};
}  // namespace s21