#include "MousePickingSystem.h"

#include "Utils.h"
#include "events/MouseDoubleClickedEvent.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

MousePickingSystem::MousePickingSystem() {
  initializeOpenGLFunctions();
  framebuffer_->Create({Format::RED_INTEGER, Format::DEFAULT_DEPTH});
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
  static auto &pos =
      scene_->GetComponent<MouseInput>(scene_->GetEntities<MouseInput>().at(0))
          .double_click;

  if (pos == QPoint{-1, -1}) return;

  auto [proj, view] = Utils::GetProjectionAndView(scene_);
  technique_->Enable(TechniqueType::MOUSE_PICKING);
  framebuffer_->Bind();
  PrepareFramebuffer();

  for (auto &entity : entities_) {
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto &model = scene_->GetComponent<Model>(entity);

    technique_->setMVP(proj, view, transform.GetModelMatrix());
    technique_->SetObjectID((int)entity);

    DrawObject(model);
  }

  framebuffer_->Unbind();

  PickEntity(pos);
}

void MousePickingSystem::PrepareFramebuffer() {
  glClearColor(-1, -1, -1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void MousePickingSystem::DrawObject(Model &model) {
  for (auto &mesh : model.meshes) {
    if (!mesh.VAO) mesh.bufferize(this);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

void MousePickingSystem::PickEntity(QPoint &pos) {
  int entity = framebuffer_->ReadPixel(pos.x(), pos.y());
  auto pickedEntities = scene_->GetEntities<PickingTag>();

  for (EntityID entity : pickedEntities)
    scene_->RemoveComponent<PickingTag>(entity);

  if (entity >= 0) scene_->AddComponent<PickingTag>(entity);

  pos = {-1, -1};

  qDebug() << entity;
}

}  // namespace s21
