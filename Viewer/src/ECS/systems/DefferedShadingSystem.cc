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
  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &material = scene_->GetComponent<Material>(entity);

    technique_->Enable(TechniqueType::DEFFERED_SHADING);
    technique_->Clear();
    technique_->setMaterial(material);
    technique_->setMVP(camera.projection_matrix, camera.view_matrix,
                       transform.GetModelMatrix());

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
