#pragma once

#include <vector>

#include "Observer.h"

namespace s21 {
class Observable {
 public:
  virtual ~Observable() {}
  void AddObserver(Observer *observer) { observers_.push_back(observer); }

 protected:
  virtual void Notify() {
    for (auto &observer : observers_) observer->Notify();
  }

  void NotifyCamera() {
    for (auto &observer : observers_) observer->OnCameraNotify();
  }

 private:
  std::vector<Observer *> observers_;
};
}  // namespace s21
