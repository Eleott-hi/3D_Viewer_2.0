#include "Components.h"

namespace s21 {

void Mesh::bufferize(QOpenGLExtraFunctions *f) {
  uint32_t VBO = 0, EBO = 0;

  f->glGenVertexArrays(1, &VAO);
  f->glGenBuffers(1, &VBO);
  f->glGenBuffers(1, &EBO);

  f->glBindVertexArray(VAO);

  f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  f->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
                  vertices.data(), GL_STATIC_DRAW);

  f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(),
                  indices.data(), GL_STATIC_DRAW);

  f->glEnableVertexAttribArray(0);
  f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                           (void *)offsetof(Vertex, position));

  f->glEnableVertexAttribArray(1);
  f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                           (void *)offsetof(Vertex, normal));

  f->glEnableVertexAttribArray(2);
  f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                           (void *)offsetof(Vertex, tex_coords));

  f->glEnableVertexAttribArray(3);
  f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                           (void *)offsetof(Vertex, tangent));

  f->glEnableVertexAttribArray(4);
  f->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                           (void *)offsetof(Vertex, bitangent));
  f->glBindVertexArray(0);

  index_count = indices.size();
  vertex_count = vertices.size();
}

void Mesh::Draw(QOpenGLExtraFunctions *f, GLenum form) {
  f->glBindVertexArray(VAO);
  f->glDrawElements(form, index_count, GL_UNSIGNED_INT, 0);
  f->glBindVertexArray(0);
}

QMatrix4x4 Transform::GetModelMatrix() const {
  QMatrix4x4 m_scale, m_translate, m_rotate, m_start;

  m_scale.scale(scale);
  m_translate.translate(translation);

  

  m_rotate.rotate(rotation);
  // m_rotate.rotate(rotation.y(), 0, 1, 0);
  // m_rotate.rotate(rotation.z(), 0, 0, 1);

  return m_translate * m_scale * m_rotate * m_start;
}

void Camera::UpdateViewMatrix() {
  QMatrix4x4 view;
  view.lookAt(position, position + front, up);
  view_matrix = view;
}

void Camera::Update() {
  static const QVector3D world_up = {0, 1, 0};

  const float yaw_ = qDegreesToRadians(yaw);
  const float pitch_ = qDegreesToRadians(pitch);

  front.setX(cos(yaw_) * cos(pitch_));
  front.setY(sin(pitch_));
  front.setZ(sin(yaw_) * cos(pitch_));
  front.normalize();

  right = QVector3D::normal(front, world_up);
  up = QVector3D::normal(right, front);

  UpdateViewMatrix();
}

void Camera::UpdateProjectionMatrix() {
  QMatrix4x4 m;
  if (!perspective)
    m.perspective(zoom, width / height, 1.0, 500.0);
  else
    m.ortho(left_clip, right_clip,  //
            bottom_clip, top_clip,  //
            near_clip, far_clip);

  projection_matrix = m;
}

}  // namespace s21