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

  auto [proj, view] = Utils::GetProjectionAndView(scene_);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);

    technique_->Enable(scene_->EntityHasComponent<Shader>(entity)
                           ? scene_->GetComponent<Shader>(entity).type
                           : TechniqueType::SIMPLE_COLOR);

    technique_->Clear();
    technique_->setMVP(proj, view, transform.GetModelMatrix());

    // technique_->setTexture(enviroment.light);

    if (scene_->EntityHasComponent<Material>(entity))
      technique_->setMaterial(scene_->GetComponent<Material>(entity));

    // DrawObject(model);
    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
