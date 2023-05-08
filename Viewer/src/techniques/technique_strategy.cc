#include "technique_strategy.h"

#include <memory>
#include <vector>

#include "StencilOutlineTechnique.h"
#include "cubemap_technique.h"
#include "light_color_technique.h"
#include "light_texture_normalmap_technique.h"
#include "light_texture_technique.h"
#include "picking_technique.h"
#include "quad_technique.h"
#include "simple_color_technique.h"
#include "simple_texture_technique.h"

namespace s21 {

TechniqueStrategy::TechniqueStrategy() noexcept
    : techniques_{
          std::make_shared<SimpleColorTechnique>(),
          std::make_shared<SimpleTextureTechnique>(),
          std::make_shared<LightColorTechnique>(),
          std::make_shared<LightTextureTechnique>(),
          std::make_shared<PickingTechnique>(),
          std::make_shared<QuadTechnique>(),
          std::make_shared<CubemapTechnique>(),
          std::make_shared<NormalMapTechnique>(),
          std::make_shared<StencilOutlineTechnique>(),
      } {}

void TechniqueStrategy::Enable(TechniqueType type) {
  technique_ = techniques_.at((int)type);
  technique_->Enable();
}

void TechniqueStrategy::setColor(QColor c) { technique_->setColor(c); }

void TechniqueStrategy::setMaterial(Material const &material) {
  technique_->setMaterial(material);
}

void TechniqueStrategy::setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                               QMatrix4x4 model) {
  technique_->setMVP(proj, view, model);
}

void TechniqueStrategy::setTexture(Texture const &texture) {
  technique_->setTexture(texture);
}

void TechniqueStrategy::SetObjectID(int ObjectID) {
  technique_->SetObjectID(ObjectID);
}

// void TechniqueStrategy::setLight(
//     QVector<std::tuple<Light *, BaseLightType *, Attenuation *>> lights) {
//   technique_->setLight(lights);
// }

void TechniqueStrategy::setLight(
    QVector<Light> lights, QVector<std::optional<Attenuation>> attenuations) {
  technique_->setLight(lights, attenuations);
}

}  // namespace s21
