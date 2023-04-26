#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QVector3D>
#include <memory>
#include <vector>

// =============== Components ===================

struct TransformComponent {
  QVector3D position;
  QVector3D rotation;
  float scale = 1;

  QMatrix4x4 normalizeMatrix_;
  bool normalize = false;

  QMatrix4x4 GetModelMatrix() const {
    QMatrix4x4 m_scale, m_translate, m_rotate, m_start;

    m_scale.scale(scale);
    m_translate.translate(position);

    m_rotate.rotate(rotation.x(), 1, 0, 0);
    m_rotate.rotate(rotation.y(), 0, 1, 0);
    m_rotate.rotate(rotation.z(), 0, 0, 1);

    if (normalize) m_start = normalizeMatrix_;

    return m_translate * m_scale * m_rotate * m_start;
  }
};

struct Vertex {
  QVector3D Position;
  QVector3D Normal;
  QVector2D TexCoords;
};

struct s_Mesh {
  uint32_t VAO = 0;
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
};

struct MeshComponent {
  std::string filename_;
  std::vector<s_Mesh> meshes_;
};

struct CameraComponent {
  QMatrix4x4 projectionMatrix;
  QMatrix4x4 viewMatrix;
};

struct PointSettingsComponent {
  bool show = false;
  bool smooth = false;
  uint32_t size = 1;
  QColor color = {Qt::red};
};

struct LineSettingsComponent {
  bool show = false;
  bool dashed = false;
  uint32_t size = 1;
  QColor color = {Qt::green};
};

enum class TechniqueType {
  SIMPLE_COLOR,
  SIMPLE_TEXTURE,
  LIGHT_COLOR,
  LIGHT_TEXTURE,
  MOUSE_PICKING
};

struct TechniqueComponent {
  TechniqueType type = TechniqueType::SIMPLE_COLOR;
};

enum class TextureType {  //
  DIFFUSE,
  SPECULAR,
  NORMAL,
  HEIGHT
};

struct LightSettingsComponent {
  QVector3D ambient;
  QVector3D diffuse;
  QVector3D specular;
  QVector3D constants;
};

struct MaterialComponent {
  float shininess = 32;
};

struct TextureComponent {
  uint32_t diffuse = 0;
  uint32_t specular = 0;
  uint32_t normal = 0;
  uint32_t height = 0;
};

// =============== Tags ===================
struct GeometryComponent {};
struct AxisComponent {};
struct PointLightComponent {};
struct PickingComponent {};
