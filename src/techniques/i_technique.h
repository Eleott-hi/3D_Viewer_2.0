// ================================================
// ===         Copyright 2022 pintoved          ===
// ================================================

#ifndef SRC_HEADERS_TECHNIQUE_H
#define SRC_HEADERS_TECHNIQUE_H

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QVector3D>

// #include "common_settings.h"
// #include "light.h"
#include "model_settings.h"

namespace s21 {

class ITechnique {
 public:
  static void GenVertexAndFragmentShader(QOpenGLShaderProgram &shader,
                                         const QString &vertexFile,
                                         const QString &fragmentFile) {
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile);
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile);
    shader.link();
  }

  ITechnique() {}
  virtual ~ITechnique() {}

  virtual void init() = 0;
  virtual void Enable() = 0;

  // =================== Optional ===================
  virtual void setColor(const QColor &c) {}
  virtual void setShininess(float shineness) {}
  virtual void SetObjectID(int ObjectID) {}
  virtual void setTexture(TextureComponent const& texture) {}
  virtual void ApplyLightSettings(
      std::vector<TransformComponent> const &transforms,
      std::vector<LightSettingsComponent> const &settings) {}
  virtual void setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                      const QMatrix4x4 &model) {}
};

}  // namespace s21

#endif  // SRC_HEADERS_TECHNIQUE_H
