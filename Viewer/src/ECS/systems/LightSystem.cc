#include "LightSystem.h"

#include <tuple>

#include "Utils.h"

namespace s21 {

void LightSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void LightSystem::Update() {
  static QVector<std::tuple<Light *, BaseLightType *, Attenuation *>> lights;
  lights.clear();

  for (auto entity : entities_) {
    Light *light = nullptr;
    BaseLightType *type = nullptr;
    Attenuation *attenuation = nullptr;

    light = &scene_->GetComponent<Light>(entity);

    if (scene_->EntityHasComponent<Attenuation>(entity))
      attenuation = &scene_->GetComponent<Attenuation>(entity);

    if (scene_->EntityHasComponent<DirectionalLight>(entity)) {
      type = &scene_->GetComponent<DirectionalLight>(entity);

    } else if (scene_->EntityHasComponent<PointLight>(entity)) {
      type = &scene_->GetComponent<PointLight>(entity);

    } else {
      type = &scene_->GetComponent<SpotLight>(entity);
    }

    lights.emplace_back(light, type, attenuation);
  }

  technique_->Enable(TechniqueType::LIGHT_COLOR);
  technique_->setLight(lights);

  // technique_->Enable(TechniqueType::LIGHT_TEXTURE);
  // technique_->ApplyLightSettings(transforms, lights);
}

}  // namespace s21
