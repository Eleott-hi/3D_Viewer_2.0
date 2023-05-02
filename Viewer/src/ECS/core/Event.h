#pragma once

#include "types.h"

namespace s21 {

enum class EventType {
  None,
  AppTick,
  KeyTyped,
  AppUpdate,
  AppRender,
  MouseMoved,
  KeyPressed,
  WindowClose,
  KeyReleased,
  WindowFocus,
  WindowMoved,
  WindowResize,
  MouseScrolled,
  MouseDoubleClicked,
  WindowLostFocus,
  MouseButtonPressed,
  MouseButtonReleased,
};

class Event {
 public:
  Event() = default;
  virtual ~Event() = default;

  virtual EventType GetType() = 0;
};

}  // namespace s21