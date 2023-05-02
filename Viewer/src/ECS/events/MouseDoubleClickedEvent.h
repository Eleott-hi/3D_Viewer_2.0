#pragma once

#include <QPoint>

#include "core/Event.h"

namespace s21 {
class MouseDoubleClickedEvent : public Event {
 public:
  MouseDoubleClickedEvent() = delete;
  MouseDoubleClickedEvent(QPoint pos) : position_(pos) {}
  ~MouseDoubleClickedEvent() = default;

  EventType GetType() override { return EventType::MouseDoubleClicked; }
  QPoint Position() const { return position_; }

  QPoint position_;

 private:
};
}  // namespace s21
