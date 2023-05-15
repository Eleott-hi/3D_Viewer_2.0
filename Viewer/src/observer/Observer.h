#pragma once

namespace s21 {
class Observer {
 public:
  virtual ~Observer() = default;
  void Notify() { OnNotify(); }
  void NotifyCamera() { OnCameraNotify(); }

  virtual void OnNotify() = 0;

  virtual void OnCameraNotify() = 0;
};
}  // namespace s21
