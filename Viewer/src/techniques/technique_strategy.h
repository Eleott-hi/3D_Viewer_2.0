#pragma once

#include <memory>
#include <vector>

#include "i_technique.h"

namespace s21 {

enum class TechniqueType {
  SIMPLE_COLOR = 0,
  // SIMPLE_TEXTURE,
  LIGHT_COLOR,
  // LIGHT_TEXTURE,
  MOUSE_PICKING,
  QUAD,
};

class TechniqueStrategy {
 public:
  TechniqueStrategy(TechniqueType type = TechniqueType::SIMPLE_COLOR);

  void Enable(TechniqueType type);
  void setColor(QColor c);
  void setMaterial(Material const &material);
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model);
  void setTextureId(uint32_t id);
  void SetObjectID(int ObjectID);
  void setLight(
      QVector<std::tuple<Light *, BaseLightType *, Attenuation *>> lights);
  ITechnique *getTechnique() { return technique_; }

  template <typename Type>
  void setCustomValue(const char *name, Type value) {
    technique_->setUniformValue(name, value);
  }

 private:
  std::vector<std::shared_ptr<ITechnique>> techniques_;
  ITechnique *technique_;
};

}  // namespace s21
