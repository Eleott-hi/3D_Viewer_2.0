#include "PointShadowRenderSystem.h"
#include "Utils.h"

namespace s21 {

void PointShadowRenderSystem::Init(ECS_Controller *scene,
                                   TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void PointShadowRenderSystem::Update(uint32_t shadowMap) {
  auto [proj, view] = Utils::GetProjectionAndView(scene_);

  for (auto entity : entities_) {
    if (!scene_->EntityHasComponent<Shader>(entity)) continue;

    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &material = scene_->GetComponent<Material>(entity);

    technique_->Enable(TechniqueType::POINT_SHADOW_RENDER);
    technique_->Clear();
    technique_->setTexture({shadowMap, "shadowMap"});
    technique_->setMaterial(material);
    technique_->setMVP(proj, view, transform.GetModelMatrix());

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }
}
}  // namespace s21
