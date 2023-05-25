#include "LightSystem.h"

#include <tuple>

#include "Utils.h"

namespace s21 {

void LightSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void LightSystem::Update() {
  QVector<Light> lights;
  QVector<std::optional<Attenuation>> attenuations;

  lights.reserve(entities_.size());
  attenuations.reserve(entities_.size());

  for (auto entity : entities_) {
    lights << scene_->GetComponent<Light>(entity);
    attenuations << std::nullopt;

    if (scene_->EntityHasComponent<Attenuation>(entity))
      attenuations.back() = scene_->GetComponent<Attenuation>(entity);

    if (scene_->EntityHasComponent<Transform>(entity))
      lights.back().position =
          scene_->GetComponent<Transform>(entity).translation;
  }

  static QVector<TechniqueType> types = {
      TechniqueType::LIGHT_COLOR,
      TechniqueType::LIGHT_TEXTURE,
      TechniqueType::NORMALMAP,
      TechniqueType::PHYSICAL_BASED_RENDERING,
  };

  for (auto type : types) {
    technique_->Enable(type);
    technique_->setLight(lights, attenuations);
  }
}



}  // namespace s21
