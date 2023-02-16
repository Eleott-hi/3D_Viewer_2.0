#ifndef SRC_HEADERS_TECHNIQUE_STRATEGY_H
#define SRC_HEADERS_TECHNIQUE_STRATEGY_H

#include <memory>
#include <vector>

#include "Components.h"
#include "i_technique.h"

namespace s21 {

class TechniqueStrategy {
 public:
  ~TechniqueStrategy() = default;
  TechniqueStrategy(TechniqueType type = TechniqueType::SIMPLE_COLOR);

  void Enable(TechniqueType type);

  void setColor(const QColor &c);

  void setShininess(float shineness);

  void setMVP(const QMatrix4x4 &proj,  //
              const QMatrix4x4 &view,  //
              const QMatrix4x4 &model);

  void setTexture(TextureComponent const &texture);

  void SetObjectID(int ObjectID);

  void ApplyLightSettings(std::vector<TransformComponent> const &transforms,
                          std::vector<LightSettingsComponent> const &settings);

  ITechnique *getTechnique() { return technique_; }

 private:
  std::vector<std::shared_ptr<ITechnique>> techniques_;
  ITechnique *technique_;
};

}  // namespace s21

#endif  // SRC_HEADERS_TECHNIQUE_STRATEGY_H
