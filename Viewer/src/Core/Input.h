#pragma once

#include <QPoint>

namespace s21 {

class Input {
 public:
  static bool IsKeyPressed(Qt::Key key);
  static void KeyPressed(Qt::Key key);
  static void KeyReleased(Qt::Key key);

  static QPoint MousePosition();
  static void MousePosition(QPoint pos);

  static bool IsMouseButtonPressed(Qt::MouseButton button);
  static void MousePressed(Qt::MouseButton button);
  static void MouseReleased(Qt::MouseButton button);

  static bool IsMouseDoubleClicked();
  static void SetDoubleClicked(bool double_click);

 private:
};
}  // namespace s21
