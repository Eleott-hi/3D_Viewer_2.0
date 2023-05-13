#include "DefferedShadingSystem.h"

#include "Utils.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

void DefferedShadingSystem::Init(ECS_Controller *scene,
                                 TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void DefferedShadingSystem::Update() {
  auto [proj, view] = Utils::GetProjectionAndView(scene_);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);

//    technique_->Enable(TechniqueType::DEFFERED_SHADING);

    technique_->Clear();
    technique_->setMVP(proj, view, transform.GetModelMatrix());

    if (scene_->EntityHasComponent<Material>(entity))
      technique_->setMaterial(scene_->GetComponent<Material>(entity));

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
