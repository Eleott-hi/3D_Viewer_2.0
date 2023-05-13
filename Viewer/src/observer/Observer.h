#pragma once

namespace s21 {
class Observer {
 public:
  virtual ~Observer() = default;
  void Notify() { OnNotify(); }

  virtual void OnNotify() = 0;
};
}  // namespace s21
