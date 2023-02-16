#ifndef SRC_HEADERS_SIMPLE_COLOR_H
#define SRC_HEADERS_SIMPLE_COLOR_H

#include "i_technique.h"

namespace s21 {

class SimpleColorTechnique : public ITechnique {
 public:
  SimpleColorTechnique() { init(); }
  ~SimpleColorTechnique() {}

  virtual void init() override;

  virtual void Enable() override { shader_.bind(); }

  //  virtual void ApplySettings(ModelSettings *mSettings) override;

  virtual void setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                      const QMatrix4x4 &model) override;

  virtual void setColor(const QColor &c) override;

 private:
  QOpenGLShaderProgram shader_;
};

}  // namespace s21

#endif  // SRC_HEADERS_SIMPLE_COLOR_H
