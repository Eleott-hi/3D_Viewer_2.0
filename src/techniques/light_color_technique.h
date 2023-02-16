#ifndef SRC_HEADERS_COLOR_TECHNIQUE_H
#define SRC_HEADERS_COLOR_TECHNIQUE_H

#include <string>

#include "i_technique.h"

namespace s21 {

class LightColorTechnique : public ITechnique {
 public:
  LightColorTechnique() { init(); }
  ~LightColorTechnique() {}

  virtual void init() override;

  virtual void Enable() override { shader_.bind(); }

  virtual void ApplyLightSettings(
      std::vector<TransformComponent> const &transforms,
      std::vector<LightSettingsComponent> const &settings) override;

  virtual void setColor(const QColor &c) override;
  virtual void setShininess(float shininess) override;
  virtual void setMVP(const QMatrix4x4 &proj,  //
                      const QMatrix4x4 &view,  //
                      const QMatrix4x4 &model) override;

 private:
  QOpenGLShaderProgram shader_;
};

}  // namespace s21

#endif  // SRC_HEADERS_COLOR_TECHNIQUE_H
