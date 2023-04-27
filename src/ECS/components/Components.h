#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QVector3D>
#include <memory>
#include <vector>

struct Vertex {
  QVector3D Position;
  QVector3D Normal;
  QVector2D TexCoords;
};

struct Mesh {
  uint32_t VAO = 0;
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;

  void bufferize(QOpenGLExtraFunctions *f) {
    uint32_t VBO = 0, EBO = 0;

    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);

    f->glBindVertexArray(VAO);

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(),
                    vertices_.data(), GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_.size(),
                    indices_.data(), GL_STATIC_DRAW);

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             (void *)offsetof(Vertex, Position));
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             (void *)offsetof(Vertex, Normal));

    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             (void *)offsetof(Vertex, TexCoords));
    f->glBindVertexArray(0);
  }
};

struct ModelComponent {
  std::string filename_;
  std::vector<Mesh> meshes_;
};

struct PositionComponent {
  QVector3D position;
};

struct RotationComponent {
  QVector3D rotation;
};

struct TransformComponent : public PositionComponent, public RotationComponent {
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

struct LightComponent {
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
struct AxisTag {};
struct GeometryTag {};
struct PickingTag {};
struct DirLightTag {};
struct SpotLightTag {};
struct PointLightTag : public PositionComponent, LightComponent {};
