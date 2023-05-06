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
  scene_->AddComponent<Texture>(texture, {textureID});
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

  technique_->Enable(TechniqueType::SIMPLE_TEXTURE);
  auto [proj, view] = Utils::GetProjectionAndView(scene_);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto &transform = scene_->GetComponent<Transform>(entity);
    auto &material = scene_->GetComponent<Material>(entity);
    auto &texture = scene_->GetComponent<Texture>(entity);

    technique_->setTextureId(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    technique_->setMVP(proj, view, transform.GetModelMatrix());
    technique_->setMaterial(material);

    DrawObject(model);
  }

  // framebuffer_->Unbind();
}

void RenderSystem::PrepareFramebuffer() {
  glClearColor(0.5, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderSystem::DrawObject(Model &model, GLenum form) {
  for (auto &mesh : model.meshes) {
    if (!mesh.VAO) mesh.bufferize(this);

    glBindVertexArray(mesh.VAO);
    glDrawElements(form, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

}  // namespace s21
