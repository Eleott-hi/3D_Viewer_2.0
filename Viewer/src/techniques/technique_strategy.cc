#include "technique_strategy.h"

#include <memory>
#include <vector>

#include "light_color_technique.h"
// #include "light_texture_technique.h"
#include "picking_technique.h"
#include "simple_color_technique.h"
// #include "simple_texture_technique.h"
#include "cubemap_technique.h"
#include "quad_technique.h"

namespace s21 {

TechniqueStrategy::TechniqueStrategy(TechniqueType type)
    : techniques_{std::make_shared<SimpleColorTechnique>(),
                  //  std::make_shared<SimpleTextureTechnique>(),
                  std::make_shared<LightColorTechnique>(),
                  //                  std::make_shared<LightTextureTechnique>(),
                  std::make_shared<PickingTechnique>(),
                  std::make_shared<QuadTechnique>(),
                  std::make_shared<CubemapTechnique>()} {
  technique_ = techniques_.at((int)type).get();
}

void TechniqueStrategy::Enable(TechniqueType type) {
  technique_ = techniques_.at((int)type).get();
  technique_->Enable();
}

void TechniqueStrategy::setColor(QColor c) { technique_->setColor(c); }

void TechniqueStrategy::setMaterial(Material const &material) {
  technique_->setMaterial(material);
}

void TechniqueStrategy::setMVP(QMatrix4x4 proj,  //
                               QMatrix4x4 view,  //
                               QMatrix4x4 model) {
  technique_->setMVP(proj, view, model);
}

void TechniqueStrategy::setTextureId(uint32_t id) {
  technique_->setTextureID(id);
}

void TechniqueStrategy::SetObjectID(int ObjectID) {
  technique_->SetObjectID(ObjectID);
}

void TechniqueStrategy::setLight(
    QVector<std::tuple<Light *, BaseLightType *, Attenuation *>> lights) {
  technique_->setLight(lights);
}

}  // namespace s21
