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
  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Mesh>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &material = scene_->GetComponent<Material>(entity);

    technique_->Enable(TechniqueType::DEFFERED_SHADING);
    technique_->Clear();
    technique_->SetMaterial(material);
    technique_->SetModelMatrix(transform.GetModelMatrix());

    model.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
