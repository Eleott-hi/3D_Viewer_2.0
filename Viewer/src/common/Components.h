#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QVector3D>
#include <QVector>
#include <string>

#include "core/ECS_Controller.h"

namespace s21 {

struct Vertex {
  QVector3D position;
  QVector3D normal;
  QVector2D tex_coords;
  QVector3D tangent;
  QVector3D bitangent;
};

struct Mesh {
  quint32 VAO = 0;
  QVector<Vertex> vertices;
  QVector<uint32_t> indices;
  int vertex_count = 0;
  int index_count = 0;

  void bufferize(QOpenGLExtraFunctions *f);
};

struct Model {
  std::string filename = "";
  QVector<Mesh> meshes;
};

struct Transform {
  QVector3D translation = {0, 0, -2};
  QVector3D rotation = {0, 0, 0};
  QVector3D scale = {1, 1, 1};

  QMatrix4x4 GetModelMatrix() const;
};

struct Material {
  QColor color = QColor::fromRgbF(0, 1, 0);

  uint32_t diffuse = 0;
  uint32_t normal = 0;

  float shininess = 32;
};

enum class LightType { DIRECTIONAL, POINT, SPOT };

struct BaseLightType {
  virtual LightType GetType() = 0;
};

struct DirectionalLight : BaseLightType {
  QVector3D direction = {0, 0, -1};

  LightType GetType() final { return LightType::DIRECTIONAL; }
};

struct PointLight : BaseLightType {
  QVector3D position = {0, 0, 0};

  LightType GetType() final { return LightType::POINT; }
};

struct SpotLight : BaseLightType {
  QVector3D position = {0, 0, 0};
  QVector3D direction = {0, 0, -1};
  float inner_cone = 12.5;
  float outer_cone = 15.0;

  LightType GetType() final { return LightType::SPOT; }
};

struct Light {
  QVector3D ambient = {0.1, 0.1, 0.1};
  QVector3D diffuse = {0.1, 0.1, 0.1};
  QVector3D specular = {0.1, 0.1, 0.1};
};

struct Attenuation {
  float constant = 0.1;
  float linear = 0.1;
  float quadratic = 0.1;
};

struct Camera {
  QVector3D position;
  QVector3D front = {0, 0, -1};
  QVector3D right = {1, 0, 0};
  QVector3D up = {0, 1, 0};

  float pitch = 0;
  float yaw = -90;
  float zoom = 45.0;

  QMatrix4x4 GetViewMatrix() const;
};

struct Projection {
  QMatrix4x4 matrix;
  bool perspective = true;
};

struct Texture {
  uint32_t id = 0;
  std::string type;
};

struct Quad {};
struct Cubemap {};

struct PickingTag {};

}  // namespace s21
