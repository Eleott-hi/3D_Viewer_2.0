
#pragma once

#include <QOpenGLExtraFunctions>
#include <QPoint>

#include "OpenGLFramebuffer.h"
#include "TechniqueStrategy.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

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
  std::unique_ptr<IFramebuffer> framebuffer_ =
      std::make_unique<OpenGLFramebuffer>();

  void PrepareFramebuffer();
  void PickEntity(QPoint pos);
  void OnWindowResize(Event &event);
};
}  // namespace s21
