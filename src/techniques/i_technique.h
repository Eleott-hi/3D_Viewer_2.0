// ================================================
// ===         Copyright 2022 pintoved          ===
// ================================================

#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>

#include "components/Components.h"
#include "model_settings.h"

namespace s21 {

class ITechnique {
public:
  ITechnique() {}
  virtual ~ITechnique() {}

  virtual void init() = 0;
  virtual void Enable() { shader_.bind(); };

  // =================== Optional ===================
  virtual void setColor(const QColor &c) {}
  virtual void SetObjectID(int ObjectID) {}
  virtual void setShininess(float shineness) {}
  virtual void setTexture(TextureComponent const &texture) {}
  virtual void
  ApplyLightSettings(std::vector<TransformComponent> const &transforms,
                     std::vector<LightSettingsComponent> const &settings) {}
  virtual void setMVP(const QMatrix4x4 &proj, //
                      const QMatrix4x4 &view, //
                      const QMatrix4x4 &model) {}

protected:
  QOpenGLShaderProgram shader_;

  void GenerateShaders(const QString &vertex_file,
                       const QString &fragment_file) {
    bool ok;
    ok = shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex_file);
    Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
               "Failed to compile vertex shader");

    ok =
        shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, fragment_file);
    Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
               "Failed to compile fragment shader");

    ok = shader_.link();
    Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
               "Failed to link shader program");
  }
};

} // namespace s21
