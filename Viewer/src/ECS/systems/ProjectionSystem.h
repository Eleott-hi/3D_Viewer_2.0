
#pragma once

#include <QPoint>
#include <QVector3D>

#include "Components.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

class ProjectionSystem : public System {
 public:
  ProjectionSystem() = default;
  ~ProjectionSystem() = default;

  void Init(ECS_Controller *scene);
  void Update();

 private:
  QMatrix4x4 ortho_, perspective_;
  ECS_Controller *scene_ = nullptr;

  void OnWindowResize(Event &event);
};
}  // namespace s21
