#include "ShadowRenderSystem.h"

#include "Utils.h"

namespace s21 {

void ShadowRenderSystem::Init(ECS_Controller *scene,
                              TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void ShadowRenderSystem::Update(uint32_t shadowMap) {
  auto [proj, view] = Utils::GetProjectionAndView(scene_);

  for (auto entity : entities_) {
    if (!scene_->EntityHasComponent<Shader>(entity)) continue;

    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &material = scene_->GetComponent<Material>(entity);

    technique_->Enable(TechniqueType::SHADOW_RENDERING);
    technique_->Clear();
    technique_->setMaterial(material);
    technique_->setTexture({shadowMap, "shadowMap"});
    technique_->setMVP(proj, view, transform.GetModelMatrix());

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
