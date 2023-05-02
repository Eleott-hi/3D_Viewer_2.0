#include "ProjectionSystem.h"

#include "events/WindowResizeEvent.h"

namespace s21 {

QMatrix4x4 GetPerspective(float width, float height) {
  QMatrix4x4 perspective;
  perspective.perspective(45.0, width / height, 1.0, 500.0);
  return perspective;
}

void ProjectionSystem::Init(ECS_Controller *scene) {
  scene_ = scene;
  scene_->AddEventListener(EventType::WindowResize,
                           BIND_EVENT_FN(OnWindowResize));

  ortho_.ortho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
}

void ProjectionSystem::OnWindowResize(Event &e) {
  auto &event = static_cast<WindowResizeEvent &>(e);
  perspective_ = GetPerspective(event.Width(), event.Height());
}

void ProjectionSystem::Update() {
  for (auto &entity : entities_) {
    auto &projection = scene_->GetComponent<Projection>(entity);
    projection.matrix = projection.perspective ? perspective_ : ortho_;
  }
}

}  // namespace s21
