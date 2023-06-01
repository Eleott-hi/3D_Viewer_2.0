#include "RenderSystem.h"

#include "Hierarchy.h"
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

  std::unordered_map<int, QMatrix4x4> matricies;
  std::function<QMatrix4x4(EntityID)> get_model_matrix = [&](EntityID entity) {
    if (matricies.find(entity) == matricies.end() &&
        scene_->EntityHasComponent<Transform>(entity)) {
      auto &transform = scene_->GetComponent<Transform>(entity);
      auto &hierarchy = scene_->GetComponent<HierarchyComponent>(entity);

      matricies[entity] =
          get_model_matrix(hierarchy.parent) * transform.GetModelMatrix();
    }

    return matricies[entity];
  };

  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));

  for (auto entity : entities_) {
    if (!scene_->EntityHasComponent<Shader>(entity)) continue;

    auto &model = scene_->GetComponent<Mesh>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &material = scene_->GetComponent<Material>(entity);

    technique_->Enable(scene_->GetComponent<Shader>(entity).type);
    technique_->Clear();
    technique_->SetMaterial(material);
    technique_->SetMVP(camera.projection_, camera.view_,
                       get_model_matrix(entity));

    // technique_->SetTexture(enviroment.light);

    model.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
