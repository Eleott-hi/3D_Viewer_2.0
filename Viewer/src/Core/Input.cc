#include "Input.h"

#include <unordered_map>

namespace s21 {
std::unordered_map<Qt::Key, bool> keys;

struct Mouse {
  QPoint position;

  std::unordered_map<Qt::MouseButton, bool> pressed;

  bool double_click = false;

} mouse;

void Input::KeyPressed(Qt::Key key) { keys[key] = true; }
void Input::KeyReleased(Qt::Key key) { keys[key] = false; }
bool Input::IsKeyPressed(Qt::Key key) { return keys[key]; }

QPoint Input::MousePosition() { return mouse.position; }
void Input::MousePosition(QPoint pos) { mouse.position = pos; }

bool Input::IsMouseButtonPressed(Qt::MouseButton button) {
  return mouse.pressed[button];
}
void Input::MousePressed(Qt::MouseButton button) {
  mouse.pressed[button] = true;
}
void Input::MouseReleased(Qt::MouseButton button) {
  mouse.pressed[button] = false;
}

bool Input::IsMouseDoubleClicked() { return mouse.double_click; }
void Input::SetDoubleClicked(bool double_click) {
  mouse.double_click = double_click;
}

}  // namespace s21
