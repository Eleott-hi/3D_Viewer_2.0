#include "ShadowSystem.h"

#include "Utils.h"

namespace s21 {

void ShadowSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void ShadowSystem::Update() {
  auto const &[proj, view] = Utils::GetProjectionAndView(scene_);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);

    technique_->Enable(TechniqueType::SHADOW_MAPPING);
    technique_->setMVP(proj, view, transform.GetModelMatrix());

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
