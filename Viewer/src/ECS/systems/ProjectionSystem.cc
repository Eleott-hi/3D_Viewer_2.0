#include "ProjectionSystem.h"

#include "events/MouseScrolledEvent.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

QMatrix4x4 GetPerspective(float ratio_or_scroll, bool scroll) {
  // static float ration_ = 0.5;
  // static float scroll_ = 45.0;

  // (scroll) ? (scroll_ = qBound(1.0, scroll_ - ratio_or_scroll, 45.0))
  //          : (ration_ = ratio_or_scroll);

  // QMatrix4x4 perspective;
  // perspective.perspective(scroll_, ration_, 1.0, 500.0);
  // return perspective;
}

void ProjectionSystem::Init(ECS_Controller *scene) {
  // scene_ = scene;
  // scene_->AddEventListener(EventType::WindowResize,
  //                          BIND_EVENT_FN(OnWindowResize));
  // scene_->AddEventListener(EventType::MouseScrolled,
  //                          BIND_EVENT_FN(OnMouseScroll));

  // ortho_.ortho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
}

void ProjectionSystem::OnMouseScroll(Event &e) {
  // auto &event = static_cast<MouseScrolledEvent &>(e);

  // perspective_ = GetPerspective(event.Scroll(), true);
}

void ProjectionSystem::OnWindowResize(Event &e) {
  // auto &event = static_cast<WindowResizeEvent &>(e);

  // perspective_ =
  //     GetPerspective((float)event.Width() / (float)event.Height(), false);
}

void ProjectionSystem::Update() {
  // for (auto &entity : entities_) {
  //   auto &projection = scene_->GetComponent<Projection>(entity);
  //   projection.matrix = projection.perspective ? perspective_ : ortho_;
  // }
}

}  // namespace s21
