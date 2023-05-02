#include "RenderSystem.h"

#include "Utils.h"

namespace s21 {

void RenderSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void RenderSystem::Update() {
  auto [proj, view] = Utils::GetProjectionAndView(scene_);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto &transform = scene_->GetComponent<Transform>(entity);
    auto &material = scene_->GetComponent<Material>(entity);
    auto modelMatrix = transform.GetModelMatrix();

    technique_->Enable(TechniqueType::LIGHT_COLOR);
    // technique_->setCustomValue("u_Model", modelMatrix);
    technique_->setMVP(proj, view, modelMatrix);

    technique_->setMaterial(material);

    DrawObject(model);
  }
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
