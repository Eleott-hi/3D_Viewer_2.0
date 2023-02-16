#pragma once

#include "types.h"

namespace s21 {

enum class EventType {
  None,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled

};

class Event {
 public:
  Event() = default;
  virtual ~Event() = default;

  virtual EventType GetType() = 0;
};

}  // namespace s21