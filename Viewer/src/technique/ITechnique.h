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

  template <typename Type>
  void setCustomValue(const char *name, Type value) {
    shader_.setUniformValue(name, value);
  }

  void Clear() { index_ = 0; }
  void Enable() { shader_.bind(); };
  void SetModelMatrix(QMatrix4x4 model);
  void SetProjectionViewMatrix(QMatrix4x4 proj, QMatrix4x4 view);

  virtual ~ITechnique() {}
  virtual void Init() = 0;

  // =================== Optional ===================
  virtual void SetObjectID(int ObjectID) {}
  virtual void SetTexture(Texture const &texture) {}
  virtual void SetMaterial(Material const &material) {}
  virtual void SetLight(QVector<Light> lights,
                        QVector<std::optional<Attenuation>> attenuations) {}

 protected:
  static QMatrix4x4 projection_, view_, model_;

  uint32_t index_ = 0;
  ShaderProgram shader_;

  virtual void OnMVPLoaded() {}
  void GenerateShaders(QString vertex_file,    //
                       QString geometry_file,  //
                       QString fragment_file = "");
};

}  // namespace s21
