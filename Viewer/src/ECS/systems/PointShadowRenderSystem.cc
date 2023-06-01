#include "PointShadowRenderSystem.h"

#include "Utils.h"

namespace s21 {

void PointShadowRenderSystem::Init(ECS_Controller *scene,
                                   TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void PointShadowRenderSystem::Update(uint32_t shadowMap) {
  for (auto entity : entities_) {
    if (!scene_->EntityHasComponent<Shader>(entity)) continue;

    auto &model = scene_->GetComponent<Mesh>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &material = scene_->GetComponent<Material>(entity);

    technique_->Enable(TechniqueType::POINT_SHADOW_RENDER);
    technique_->Clear();
    technique_->SetTexture({shadowMap, "shadowMap"});
    technique_->SetMaterial(material);
    technique_->SetModelMatrix(transform.GetModelMatrix());

    model.Draw(this, GL_TRIANGLES);
  }
}
}  // namespace s21
