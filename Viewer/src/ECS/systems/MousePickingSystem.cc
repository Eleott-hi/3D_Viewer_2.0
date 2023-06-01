#include "MousePickingSystem.h"

#include "Input.h"
#include "Utils.h"
#include "events/MouseDoubleClickedEvent.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

MousePickingSystem::MousePickingSystem() {
  initializeOpenGLFunctions();
  framebuffer_->Create({Format::RED_INTEGER, Format::DEFAULT_DEPTH});
  framebuffer_->SetPrepereBuffer([this] {
    glClearColor(-1, -1, -1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  });
}

void MousePickingSystem::Init(ECS_Controller *scene,
                              TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
  scene_->AddEventListener(EventType::WindowResize,
                           BIND_EVENT_FN(OnWindowResize));
}

void MousePickingSystem::OnWindowResize(Event &e) {
  auto &event = static_cast<WindowResizeEvent &>(e);
  framebuffer_->Resize(event.Width(), event.Height());
}

void MousePickingSystem::Update() {
  if (Input::MouseDoubleClick() == false) return;

  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));
  technique_->Enable(TechniqueType::MOUSE_PICKING);
  framebuffer_->Bind();
  framebuffer_->PrepereBuffer();

  for (auto &entity : entities_) {
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto &model = scene_->GetComponent<Mesh>(entity);
    // auto &heirarchy = scene_->GetComponent<HierarchyComponent>(entity);

    technique_->SetMVP(camera.projection_matrix, camera.view_matrix,
                       transform.GetModelMatrix());
    technique_->SetObjectID((int)entity);

    model.Draw(this, GL_TRIANGLES);
  }

  framebuffer_->Unbind();

  PickEntity(Input::MousePosition());

  Input::MouseDoubleClick(false);
}

void MousePickingSystem::PickEntity(QPoint pos) {
  int entity = framebuffer_->ReadPixel(pos.x(), pos.y());
  auto pickedEntities = scene_->GetEntities<PickingTag>();

  if (!Input::IsKeyPressed(Qt::Key_Control))
    for (EntityID entity : pickedEntities)
      scene_->RemoveComponent<PickingTag>(entity);

  if (entity >= 0 && std::find(pickedEntities.begin(), pickedEntities.end(),
                               entity) == pickedEntities.end())
    scene_->AddComponent<PickingTag>(entity);

  //  pos = {-1, -1};
}

}  // namespace s21
