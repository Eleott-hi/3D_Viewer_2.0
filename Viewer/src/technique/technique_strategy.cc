#include "technique_strategy.h"

#include <memory>
#include <vector>

#include "techniques/DefferedShadingTechnique.h"
#include "techniques/GizmoTechnique.h"
#include "techniques/PhysicalBasedRenderingTechnique.h"
#include "techniques/PointShadowRenderTechnique.h"
#include "techniques/PointShadowTechnique.h"
#include "techniques/ShadowMappingTechnique.h"
#include "techniques/ShadowRenderTechnique.h"
#include "techniques/StencilOutlineTechnique.h"
#include "techniques/cubemap_technique.h"
#include "techniques/light_color_technique.h"
#include "techniques/light_texture_technique.h"
#include "techniques/normalmap_technique.h"
#include "techniques/picking_technique.h"
#include "techniques/quad_technique.h"
#include "techniques/simple_color_technique.h"
#include "techniques/simple_texture_technique.h"

namespace s21 {

TechniqueStrategy::TechniqueStrategy() noexcept
    : techniques_{
          std::make_shared<SimpleColorTechnique>(),
          std::make_shared<SimpleTextureTechnique>(),
          std::make_shared<LightColorTechnique>(),
          std::make_shared<LightTextureTechnique>(),
          std::make_shared<PhysicalBasedRenderingTechnique>(),
          std::make_shared<PickingTechnique>(),
          std::make_shared<QuadTechnique>(),
          std::make_shared<CubemapTechnique>(),
          std::make_shared<NormalMapTechnique>(),
          std::make_shared<StencilOutlineTechnique>(),
          std::make_shared<DefferedShadingTechnique>(),
          std::make_shared<ShadowMappingTechnique>(),
          std::make_shared<ShadowRenderTechnique>(),
          std::make_shared<PointShadowTechnique>(),
          std::make_shared<PointShadowRenderTechnique>(),
          std::make_shared<GizmoTechnique>(),
      } {}

void TechniqueStrategy::Enable(TechniqueType type) {
  technique_ = techniques_.at((int)type);
  technique_->Enable();
}

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

void TechniqueStrategy::setLight(
    QVector<Light> lights, QVector<std::optional<Attenuation>> attenuations) {
  technique_->setLight(lights, attenuations);
}

}  // namespace s21
