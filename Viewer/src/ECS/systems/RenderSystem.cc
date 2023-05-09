#include "RenderSystem.h"

#include "Utils.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

RenderSystem::RenderSystem() {
  initializeOpenGLFunctions();
  framebuffer_->Create({Format::RGB, Format::DEFAULT_DEPTH});
}

void RenderSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;

  EntityID texture = scene_->NewEntity();
  auto textureID = framebuffer_->getTextureID();
  scene_->AddComponent<Quad>(texture);
  scene_->AddComponent<Texture>(texture, {textureID, "quad"});
  scene_->AddEventListener(EventType::WindowResize,
                           BIND_EVENT_FN(OnWindowResize));
}

void RenderSystem::OnWindowResize(Event &e) {
  auto &event = static_cast<WindowResizeEvent &>(e);
  framebuffer_->Resize(event.Width(), event.Height());
}

void RenderSystem::Update() {
  framebuffer_->Bind();
  PrepareFramebuffer();

  std::tie(proj_, view_) = Utils::GetProjectionAndView(scene_);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<NewModel>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);

    technique_->Enable(scene_->EntityHasComponent<Shader>(entity)
                           ? scene_->GetComponent<Shader>(entity).type
                           : TechniqueType::SIMPLE_COLOR);

    DrawObject(model);
  }
}

void RenderSystem::PrepareFramebuffer() {
  glClearColor(0.1, 0.1, 0.1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderSystem::DrawObject(NewModel &model, GLenum form) {
  for (auto objectID : model.meshes)
    if (scene_->EntityHasComponent<Mesh>(objectID))
      DrawMesh(scene_->GetComponent<Mesh>(objectID), form);
    else
      DrawObject(scene_->GetComponent<NewModel>(objectID), form);
}

void RenderSystem::DrawMesh(Mesh &mesh, GLenum form) {
  technique_->Clear();

  if (scene_->EntityHasComponent<Material>(entity))
    technique_->setMaterial(scene_->GetComponent<Material>(entity));

  technique_->setMVP(proj_, view_, mesh.GetModelMatrix());

  glBindVertexArray(mesh.VAO);
  glDrawElements(form, mesh.indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

}  // namespace s21
