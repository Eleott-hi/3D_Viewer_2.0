#pragma once

#include "components/Components.h"
#include "core/Event.h"

namespace s21 {
class TransformChangedEvent : public Event {
 public:
  TransformChangedEvent() {}
  ~TransformChangedEvent() = default;

  EventType GetType() override { return EventType::WindowResize; }

  TransformCompomemt GetTransform() {}

 private:
  TransformCompomemt transform_;
};
}  // namespace s21