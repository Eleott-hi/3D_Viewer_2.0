#ifndef SRC_HEADERS_LIGHT_H
#define SRC_HEADERS_LIGHT_H

#include <QOpenGLShaderProgram>
#include <QVector3D>

#include "model.h"

namespace s21 {

enum class LightType { DIRECTIONAL, POINT, SPOT };

class Light : public Model {
 public:
  ~Light() = default;
  Light(Light &&) = default;
  Light(const Light &) = default;
  Light &operator=(Light &&) = default;
  Light &operator=(const Light &) = default;

  Light() : Light("resources/cube.obj") {}

  Light(const std::string &path, bool gamma = false) : Model(path, gamma) {
    mSettings_.Scale(0.5);
    mSettings_.Translate({10, 0, 10});
    mSettings_.setVertexColor(Qt::yellow);
  }

  void setAmbient(const QVector3D &value) { ambient_ = value; }
  void setDiffuse(const QVector3D &value) { diffuse_ = value; }
  void setSpecular(const QVector3D &value) { specular_ = value; }
  void setConstant(float value) { constant_ = value; }
  void setLinear(float value) { linear_ = value; }
  void setQuadratic(float value) { quadratic_ = value; }

  LightType getType() { return type; }
  const QVector3D &getAmbient() { return ambient_; }
  const QVector3D &getDiffuse() { return diffuse_; }
  const QVector3D &getSpecular() { return specular_; }
  float getConstant() { return constant_; }
  float getLinear() { return linear_; }
  float getQuadratic() { return quadratic_; }

 private:
  QVector3D ambient_ = {0.2, 0.2, 0.2}, diffuse_ = {0.8, 0.8, 0.8},
            specular_ = {1, 1, 1};
  float constant_ = 1.0f, linear_ = 0.007f, quadratic_ = 0.0f;
  LightType type = LightType::POINT;
};

}  // namespace s21

#endif  // SRC_HEADERS_LIGHT_H
