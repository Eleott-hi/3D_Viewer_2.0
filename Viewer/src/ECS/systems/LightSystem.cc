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

  static QVector<TechniqueType> types = {TechniqueType::LIGHT_COLOR,
                                         TechniqueType::LIGHT_TEXTURE,
                                         TechniqueType::NORMALMAP};

  for (auto type : types) {
    technique_->Enable(type);
    technique_->setLight(lights, attenuations);
  }

}

// void LightSystem::Update() {
//   for (auto entity : entities_) {
//     if (scene_->EntityHasComponent<SpotLight>(entity)) {
//       // Light *light = &scene_->GetComponent<Light>(entity);
//     }

//     else if (scene_->EntityHasComponent<PointLight>(entity)) {
//       // Light *light = &scene_->GetComponent<Light>(entity);
//     }

//     else {
//     }
//   }
// }

// bool LightSystem::isSpotLight(EntityID entity) {
//   return scene_->EntityHasComponent<Transform>(entity) &&
//          scene_->EntityHasComponent<Direction>(entity) &&
//          scene_->EntityHasComponent<Cutoff>(entity);
// }

// bool LightSystem::isPointLight(EntityID entity) {
//   return scene_->EntityHasComponent<Transform>(entity);
// }

}  // namespace s21
