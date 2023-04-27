#include "MousePickingSystem.h"

#include "events/WindowResizeEvent.h"

namespace s21 {

MousePickingSystem::MousePickingSystem() {
  initializeOpenGLFunctions();
  pickingFramebuffer_ = std::make_unique<Framebuffer>();
  pickingFramebuffer_->Create({Format::RED_INTEGER, Format::DEFAULT_DEPTH});
}

void MousePickingSystem::Init(
    std::shared_ptr<ECS_Controller> const &scene,
    std::shared_ptr<TechniqueStrategy> const &technique) {
  scene_ = scene;
  technique_ = technique;
  scene_->AddEventListener(EventType::WindowResize,
                           BIND_EVENT_FN(OnWindowResize));
}

void MousePickingSystem::OnWindowResize(Event &event) {
  auto &cast_event = static_cast<WindowResizeEvent &>(event);
  pickingFramebuffer_->Resize(cast_event.GetWidth(), cast_event.GetHeight());
}

bool MousePickingSystem::Update(EntityID camera, const QPoint &pos) {
  auto &[proj, view] = scene_->GetComponent<CameraComponent>(camera);

  pickingFramebuffer_->Bind();
  glClearColor(-1, -1, -1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for (auto &entity : entities_) {
    auto const &transform = scene_->GetComponent<TransformComponent>(entity);
    auto &model = scene_->GetComponent<ModelComponent>(entity);

    QMatrix4x4 modelMatrix = transform.GetModelMatrix();

    technique_->Enable(TechniqueType::MOUSE_PICKING);
    technique_->setMVP(proj, view, modelMatrix);
    technique_->SetObjectID((int)entity);

    DrawObject(model);
  }
  pickingFramebuffer_->Unbind();

  int objectID = pickingFramebuffer_->ReadPixel(pos.x(), pos.y());

  auto pickedEntities = scene_->GetEntities<PickingTag>();

  for (auto &entity : pickedEntities)
    scene_->RemoveComponent<PickingTag>(entity);

  if (objectID >= 0) scene_->AddComponent<PickingTag>(objectID);

  qDebug() << objectID;

  return objectID >= 0;
}

void MousePickingSystem::DrawObject(ModelComponent &model) {
  for (auto &mesh : model.meshes_) {
    if (!mesh.VAO) mesh.bufferize(this);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

}  // namespace s21
