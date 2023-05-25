#pragma once
#include <QElapsedTimer>

#include "Components.h"
#include "Utils.h"
#include "core/ECS_Controller.h"
#include "core/System.h"

namespace s21 {
class TimeTickSystem : public System {
 public:
  TimeTickSystem() = default;
  ~TimeTickSystem() = default;

  void Init(ECS_Controller *scene) {
    scene_ = scene;
    timer_.start();
  }

  void Update() {
    for (auto entity : entities_) {
      auto &timer = scene_->GetComponent<Timer>(entity);

      timer.time = timer_.nsecsElapsed() / 1000000.0;

      qDebug() << timer.time << "ms";

      timer_.start();
    }
  }

 private:
  ECS_Controller *scene_ = nullptr;
  QElapsedTimer timer_;
};
}  // namespace s21
