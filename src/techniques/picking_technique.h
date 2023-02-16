// ================================================
// ===         Copyright 2022 pintoved          ===
// ================================================

#ifndef SRC_MODULES_PICKING_TECHNIQUE_H
#define SRC_MODULES_PICKING_TECHNIQUE_H

#include "i_technique.h"

namespace s21 {

class PickingTechnique : public ITechnique {
 public:
  PickingTechnique() { init(); }
  ~PickingTechnique() = default;

  virtual void init() override {
    GenVertexAndFragmentShader(shader_, "shaders/picking_shader_v.c",
                               "shaders/picking_shader_f.c");
  }

  virtual void Enable() override { shader_.bind(); }

  virtual void setMVP(const QMatrix4x4 &proj, const QMatrix4x4 &view,
                      const QMatrix4x4 &model) override {
    shader_.setUniformValue("u_MVP", proj * view * model);
  }

  virtual void SetObjectID(int ObjectID) override {
    shader_.setUniformValue("u_ObjectID", ObjectID);
  }

 private:
  QOpenGLShaderProgram shader_;
};

}  // namespace s21

#endif  // SRC_MODULES_PICKING_TECHNIQUE_H
