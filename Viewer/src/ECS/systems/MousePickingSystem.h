
#pragma once

#include <QOpenGLExtraFunctions>
#include <QPoint>

#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"
#include "framebuffer.h"
#include "technique_strategy.h"

namespace s21 {

class MousePickingSystem : public System, protected QOpenGLExtraFunctions {
 public:
  MousePickingSystem();
  ~MousePickingSystem() = default;

  void Init(ECS_Controller *scene, TechniqueStrategy *technique);
  void Update();

 private:
  QPoint clicked_pos_ = QPoint(-1, -1);
  ECS_Controller *scene_;
  TechniqueStrategy *technique_;
  std::unique_ptr<IFramebuffer> framebuffer_ = std::make_unique<Framebuffer>();

  void PrepareFramebuffer();
  void PickEntity(QPoint &pos);
  void OnWindowResize(Event &event);
};
}  // namespace s21
