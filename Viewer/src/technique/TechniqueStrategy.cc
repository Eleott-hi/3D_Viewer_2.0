#include "TechniqueStrategy.h"

#include <memory>
#include <vector>

#include "techniques/CubemapTechnique.h"
#include "techniques/DefferedShadingTechnique.h"
#include "techniques/GizmoTechnique.h"
#include "techniques/LightColorTechnique.h"
#include "techniques/LightTextureTechnique.h"
#include "techniques/NormalMapTechnique.h"
#include "techniques/PhysicalBasedRenderingTechnique.h"
#include "techniques/PickingTechnique.h"
#include "techniques/PointShadowRenderTechnique.h"
#include "techniques/PointShadowTechnique.h"
#include "techniques/QuadTechnique.h"
#include "techniques/ShadowMappingTechnique.h"
#include "techniques/ShadowRenderTechnique.h"
#include "techniques/SimpleColorTechnique.h"
#include "techniques/SimpleTextureTechnique.h"
#include "techniques/StencilOutlineTechnique.h"

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

void TechniqueStrategy::SetMaterial(Material const &material) {
  technique_->SetMaterial(material);
}

void TechniqueStrategy::SetProjectionViewMatrix(QMatrix4x4 proj,
                                                QMatrix4x4 view) {
  technique_->SetProjectionViewMatrix(proj, view);
}

void TechniqueStrategy::SetModelMatrix(QMatrix4x4 model) {
  technique_->SetModelMatrix(model);
}

void TechniqueStrategy::SetTexture(Texture const &texture) {
  technique_->SetTexture(texture);
}

void TechniqueStrategy::SetObjectID(int ObjectID) {
  technique_->SetObjectID(ObjectID);
}

void TechniqueStrategy::SetLight(
    QVector<Light> lights, QVector<std::optional<Attenuation>> attenuations) {
  technique_->SetLight(lights, attenuations);
}

}  // namespace s21
