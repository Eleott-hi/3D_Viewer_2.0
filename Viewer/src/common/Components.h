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
  POINT_SHADOW_RENDER,
  GIZMO,
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
  QQuaternion rotation;
  // QVector3D rotation;
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
  QVector3D position = {0, 0, 0};
  QVector3D front = {0, 0, -1};
  QVector3D right = {1, 0, 0};
  QVector3D up = {0, 1, 0};

  float zoom = 45.0;
  float yaw = -90, pitch = 0;
  bool constrain_pitch = true;

  uint32_t width = 800, height = 600;

  float left_clip = -1, right_clip = 1;
  float bottom_clip = -1, top_clip = 1;
  float near_clip = 1, far_clip = 1000;

  float speed = 0.004;
  float mouse_sensitivity = 0.1;

  QMatrix4x4 view_matrix;
  QMatrix4x4 projection_matrix;

  bool primary = true;
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

struct Input {
  bool left_button_pressed = false;
  std::unordered_map<int, bool> keys;
  QPoint mouse_pos;
  QPoint start;
  QPoint end;
  QPoint double_click = {-1, -1};
};

struct Timer {
  float time = 0;
};

struct QuadTag {};
struct ShadowTag {};
struct RenderTag {};
struct PickingTag {};
struct CubemapTag {};

}  // namespace s21
