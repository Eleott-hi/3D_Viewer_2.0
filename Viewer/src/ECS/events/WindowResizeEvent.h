#pragma once

#include "core/Event.h"

namespace s21 {
class WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(uint32_t width, uint32_t height)
      : width_(width), height_(height) {}
  ~WindowResizeEvent() = default;

  EventType GetType() override { return EventType::WindowResize; }
  uint32_t Width() const { return width_; }
  uint32_t Height() const { return height_; }

 private:
  uint32_t width_ = 0;
  uint32_t height_ = 0;
};
}  // namespace s21