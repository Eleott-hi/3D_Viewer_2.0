#pragma once

#include <memory>
#include <vector>

#include "ITechnique.h"

namespace s21 {

class TechniqueStrategy {
 public:
  TechniqueStrategy() noexcept;
  ~TechniqueStrategy() = default;

  template <typename Type>
  void setCustomValue(const char *name, Type value) {
    technique_->setCustomValue(name, value);
  }

  void Clear() { technique_->Clear(); }
  //  void setColor(QColor c);
  void SetObjectID(int ObjectID);
  void Enable(TechniqueType type);
  void SetMaterial(Material const &material);
  void SetTexture(Texture const &texture);
  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model);
  ITechnique *getTechnique() { return technique_.get(); }
  void SetLight(QVector<Light> lights,
                QVector<std::optional<Attenuation>> attenuations);

 private:
  std::shared_ptr<ITechnique> technique_;
  std::vector<std::shared_ptr<ITechnique>> techniques_;
};

}  // namespace s21
