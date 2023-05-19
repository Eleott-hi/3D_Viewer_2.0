// ================================================
// ===         Copyright 2022 pintoved          ===
// ================================================

#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QVector3D>
#include <QVector>

#include "Components.h"
#include "ShaderProgram.h"

namespace s21 {

class ITechnique : protected QOpenGLExtraFunctions {
 public:
  ITechnique() { initializeOpenGLFunctions(); }
  virtual ~ITechnique() {}

  virtual void init() = 0;
  virtual void Enable() { shader_.bind(); };

  // =================== Optional ===================
  virtual void Clear() {}
  virtual void SetObjectID(int ObjectID) {}
  virtual void setTexture(Texture const &texture) {}
  virtual void setMaterial(Material const &material) {}
  virtual void setLight(QVector<Light> lights,
                        QVector<std::optional<Attenuation>> attenuations) {}

  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) {}

  template <typename Type>
  void setCustomValue(const char *name, Type value) {
    qDebug() << "setCustomValue";
    shader_.setUniformValue(name, value);
  }

 protected:
  ShaderProgram shader_;

  void GenerateShaders(QString vertex_file, QString fragment_file) {
    shader_.GenerateShaders(vertex_file, fragment_file);
  }
};

}  // namespace s21
