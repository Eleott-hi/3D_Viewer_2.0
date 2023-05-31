#include "RenderSystem.h"

#include "Utils.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

void RenderSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void RenderSystem::Update() {
  // static auto &enviroment =
  // scene_->GetComponent<Enviroment>(scene_->GetEntities<Enviroment>().at(0));

  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));

  for (auto entity : entities_) {
    if (!scene_->EntityHasComponent<Shader>(entity)) continue;

    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &material = scene_->GetComponent<Material>(entity);

    technique_->Enable(scene_->GetComponent<Shader>(entity).type);
    technique_->Clear();
    technique_->SetMaterial(material);
    technique_->SetMVP(camera.projection_matrix, camera.view_matrix,
                       transform.GetModelMatrix());

    // technique_->SetTexture(enviroment.light);

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);

    model.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
