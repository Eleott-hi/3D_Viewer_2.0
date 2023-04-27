#include "technique_strategy.h"

#include <memory>
#include <vector>

#include "light_color_technique.h"
#include "light_texture_technique.h"
#include "picking_technique.h"
#include "simple_color_technique.h"
#include "simple_texture_technique.h"

namespace s21 {

TechniqueStrategy::TechniqueStrategy(TechniqueType type)
    : techniques_{std::make_shared<SimpleColorTechnique>(),
                  std::make_shared<SimpleTextureTechnique>(),
                  std::make_shared<LightColorTechnique>(),
                  std::make_shared<LightTextureTechnique>(),
                  std::make_shared<PickingTechnique>()} {
  technique_ = techniques_.at((int)type).get();
}

void TechniqueStrategy::Enable(TechniqueType type) {
  technique_ = techniques_.at((int)type).get();
  technique_->Enable();
}

void TechniqueStrategy::setColor(const QColor &c) { technique_->setColor(c); }

void TechniqueStrategy::setMaterial(MaterialComponent const &material) {
  technique_->setMaterial(material);
}

void TechniqueStrategy::setMVP(const QMatrix4x4 &proj,  //
                               const QMatrix4x4 &view,  //
                               const QMatrix4x4 &model) {
  technique_->setMVP(proj, view, model);
}

void TechniqueStrategy::setTexture(TextureComponent const &texture) {
  technique_->setTexture(texture);
}

void TechniqueStrategy::SetObjectID(int ObjectID) {
  technique_->SetObjectID(ObjectID);
}

void TechniqueStrategy::ApplyLightSettings(
    std::vector<TransformComponent> const &transforms,
    std::vector<LightComponent> const &settings) {
  technique_->ApplyLightSettings(transforms, settings);
}

}  // namespace s21
