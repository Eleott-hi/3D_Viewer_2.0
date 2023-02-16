
#pragma once

#include <QOpenGLExtraFunctions>

#include "WindowResizeEvent.h"
#include "core/Controller.h"
#include "core/System.h"
#include "framebuffer.h"
#include "technique_strategy.h"

namespace s21 {

class MousePickingSystem : public System, protected QOpenGLExtraFunctions {
 public:
  MousePickingSystem();
  ~MousePickingSystem() = default;

  void Init(std::shared_ptr<Controller> const &scene,
            std::shared_ptr<TechniqueStrategy> const &technique);

  void OnWindowResize(Event &event);
  bool Update(EntityID camera, const QPoint &pos);

 private:
  std::shared_ptr<Controller> scene_;
  std::shared_ptr<TechniqueStrategy> technique_;
  std::unique_ptr<IFramebuffer> pickingFramebuffer_;

  void DrawObject(MeshComponent &model);
  void bufferize(s_Mesh &mesh);
};
}  // namespace s21
