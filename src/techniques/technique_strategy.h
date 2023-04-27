#pragma once

#include <memory>
#include <vector>

#include "components/Components.h"
#include "i_technique.h"

namespace s21 {

class TechniqueStrategy {
 public:
  ~TechniqueStrategy() = default;
  TechniqueStrategy(TechniqueType type = TechniqueType::SIMPLE_COLOR);

  void Enable(TechniqueType type);
  void setColor(const QColor &c);
  void setMaterial(MaterialComponent const &material);
  void setMVP(const QMatrix4x4 &proj,  //
              const QMatrix4x4 &view,  //
              const QMatrix4x4 &model);
  void setTexture(TextureComponent const &texture);
  void SetObjectID(int ObjectID);
  void ApplyLightSettings(std::vector<TransformComponent> const &transforms,
                          std::vector<LightComponent> const &settings);
  ITechnique *getTechnique() { return technique_; }

 private:
  std::vector<std::shared_ptr<ITechnique>> techniques_;
  ITechnique *technique_;
};

}  // namespace s21
