#ifndef SRC_HEADERS_TECHNIQUE_STRATEGY_H
#define SRC_HEADERS_TECHNIQUE_STRATEGY_H

#include <memory>
#include <vector>

#include "Components.h"
#include "light_color_technique.h"
#include "light_texture_technique.h"
#include "picking_technique.h"
#include "simple_color_technique.h"
#include "simple_texture_technique.h"

namespace s21 {

class TechniqueStrategy {
 private:
  std::vector<std::shared_ptr<ITechnique>> techniques_ = {
      std::make_shared<SimpleColorTechnique>(),
      std::make_shared<SimpleTextureTechnique>(),
      std::make_shared<LightColorTechnique>(),
      std::make_shared<LightTextureTechnique>(),
      std::make_shared<PickingTechnique>()};
  ITechnique *technique_;

 public:
  ~TechniqueStrategy() = default;
  TechniqueStrategy(TechniqueType type = TechniqueType::SIMPLE_COLOR) {
    technique_ = techniques_.at((int)type).get();
  }

  void Enable(TechniqueType type) {
    technique_ = techniques_.at((int)type).get();
    technique_->Enable();
  }

  void setColor(const QColor &c) { technique_->setColor(c); }

  void setShininess(float shineness) { technique_->setShininess(shineness); }

  void setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
              const QMatrix4x4 &model) {
    technique_->setMVP(proj, view, model);
  }

  void setTexture(TextureComponent const &texture) {
    technique_->setTexture(texture);
  }

  void SetObjectID(int ObjectID) { technique_->SetObjectID(ObjectID); }

  void ApplyLightSettings(std::vector<TransformComponent> const &transforms,
                          std::vector<LightSettingsComponent> const &settings) {
    technique_->ApplyLightSettings(transforms, settings);
  }

  ITechnique *getTechnique() { return technique_; }
};

}  // namespace s21

#endif  // SRC_HEADERS_TECHNIQUE_STRATEGY_H
