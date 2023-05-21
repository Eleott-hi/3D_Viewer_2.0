#pragma once

#include <QColor>
#include <QImage>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QVector>
#include <string>
#include <unordered_map>

#include "core/ECS_Controller.h"

namespace s21 {

enum class TechniqueType {
  SIMPLE_COLOR,
  SIMPLE_TEXTURE,
  LIGHT_COLOR,
  LIGHT_TEXTURE,
  PHYSICAL_BASED_RENDERING,
  MOUSE_PICKING,
  QUAD,
  CUBEMAP,
  NORMALMAP,
  STENCIL_OUTLINE,
  DEFFERED_SHADING,
  SHADOW_MAPPING,
  SHADOW_RENDERING,
  POINT_SHADOW,
};

enum class LightType {
  DIRECTIONAL,
  POINT,
  SPOT,
};

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
  void Draw(QOpenGLExtraFunctions *f, GLenum form);
};

struct Model {
  std::string filename;
  QVector<Mesh> meshes;
};

struct Transform {
  QVector3D translation;
  QVector3D rotation;
  QVector3D scale = {1, 1, 1};

  QMatrix4x4 GetModelMatrix() const;
};

struct Texture {
  uint32_t id = 0;
  std::string type;
  QImage image;
  QString filename;
};

struct Material {
  QColor color = QColor::fromRgbF(0, 0.5, 0);

  Texture diffuse;
  Texture normal;
  Texture specular;
  Texture roughness;
  Texture metallic;
  Texture ao;

  float shininess = 32;
};

struct Shader {
  TechniqueType type = TechniqueType::SIMPLE_COLOR;
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

struct Light {
  LightType type = LightType::DIRECTIONAL;

  QVector3D ambient = {0.1, 0.1, 0.1};
  QVector3D diffuse = {0.1, 0.1, 0.1};
  QVector3D specular = {0.1, 0.1, 0.1};

  QVector3D position = {0, 0, 0};
  QVector3D direction = {0, 0, -1};

  float outer_cone = 15.0;
  float inner_cone = 12.5;
};

struct Enviroment {
  Texture light;
};

struct InputTag {};

struct KeyboardInput {
  std::unordered_map<int, bool> keys;
};

struct MouseInput {
  bool left_button_pressed = false;
  QPoint start;
  QPoint end;
  QPoint double_click = {-1, -1};
};

struct QuadTag {};
struct CubemapTag {};
struct PickingTag {};
struct RenderTag {};
struct ShadowTag {};

}  // namespace s21
