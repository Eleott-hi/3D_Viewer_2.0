#ifndef SRC_HEADERS_SIMPLE_TEXTURE_H
#define SRC_HEADERS_SIMPLE_TEXTURE_H

#include <QOpenGLExtraFunctions>

#include "i_technique.h"

namespace s21 {

class SimpleTextureTechnique : public ITechnique,
                               protected QOpenGLExtraFunctions {
 public:
  SimpleTextureTechnique();
  ~SimpleTextureTechnique() {}

  virtual void init() override;
  virtual void Enable() override { shader_.bind(); }

  virtual void setTexture(TextureComponent const &texture) override;

  virtual void setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                      const QMatrix4x4 &model) override;

 private:
  QOpenGLShaderProgram shader_;
};

}  // namespace s21

#endif  // SRC_HEADERS_SIMPLE_TEXTURE_H
