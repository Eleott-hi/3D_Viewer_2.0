#include "ShadowRenderSystem.h"

#include "Utils.h"

namespace s21 {

void ShadowRenderSystem::Init(ECS_Controller *scene,
                              TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void ShadowRenderSystem::Update(uint32_t shadowMap) {
  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));

  for (auto entity : entities_) {
    if (!scene_->EntityHasComponent<Shader>(entity)) continue;

    auto &model = scene_->GetComponent<Mesh>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &material = scene_->GetComponent<Material>(entity);

    technique_->Enable(TechniqueType::SHADOW_RENDERING);
    technique_->Clear();
    technique_->SetMaterial(material);
    technique_->SetTexture({shadowMap, "shadowMap"});
    technique_->SetMVP(camera.projection_, camera.view_,
                       transform.GetModelMatrix());

    model.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
