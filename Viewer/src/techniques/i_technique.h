// ================================================
// ===         Copyright 2022 pintoved          ===
// ================================================

#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QVariant>
#include <QVector3D>
#include <vector>

#include "Components.h"
// #include "model_settings.h"

namespace s21 {

class ITechnique : public QOpenGLShaderProgram {
 public:
  ITechnique() {}
  virtual ~ITechnique() {}

  virtual void init() = 0;
  virtual void Enable() { shader_.bind(); };

  // =================== Optional ===================
  virtual void setColor(QColor c) {}
  virtual void SetObjectID(int ObjectID) {}

  // virtual void setTexture(Texture id) override;
  virtual void setMaterial(Material const &material) {}
  //  virtual void setTexture(Texture const &texture) {}
  virtual void setLight(
      QVector<std::tuple<Light *, BaseLightType *, Attenuation *>> lights) {}
  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) {}

  template <typename Type>
  void setCustomValue(const char* name, Type value) {
    qDebug()<<"HERE";
   shader_.setUniformValue(name, value);
  }

 protected:
  QOpenGLShaderProgram shader_;

  void GenerateShaders(QString vertex_file, QString fragment_file) {
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

}  // namespace s21