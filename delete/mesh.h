#ifndef MY_MESH
#define MY_MESH

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector2D>
#include <QVector3D>
#include <vector>

#include "Components.h"

namespace s21 {

class Mesh : protected QOpenGLExtraFunctions {
 public:
  ~Mesh() = default;
  Mesh(Mesh &&) = default;
  Mesh(const Mesh &) = default;
  Mesh &operator=(Mesh &&) = default;
  Mesh &operator=(const Mesh &) = default;

  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned> &indices,
       const std::vector<TextureComponent> &textures)
      : vertices_(vertices), indices_(indices), textures_(textures) {
    initializeOpenGLFunctions();
  }

  s_Mesh GetMesh() {  //
    return {0,  vertices_, indices_};
  }

  void Draw() {
    if (!bufferized_) bufferize();
    Q_ASSERT(VAO_);

    // if (!textures_.empty()) BindTexture();

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // glActiveTexture(GL_TEXTURE0);
  }

 private:
  void BindTexture() {
    // for (unsigned int i = 0; i < textures_.size(); i++) {
    //   glActiveTexture(GL_TEXTURE0 + i);

    //   switch (textures_[i].type) {
    //     case TextureType::DIFFUSE:
    //       shader->setUniformValue("material.diffuse", i);
    //       break;
    //     case TextureType::SPECULAR:
    //       shader->setUniformValue("material.specular", i);
    //       break;
    //     case TextureType::NORMAL:
    //       shader->setUniformValue("material.normal", i);
    //       break;
    //     case TextureType::HEIGHT:
    //       shader->setUniformValue("material.height", i);
    //       break;
    //     default:
    //       break;
    //   }
    //   glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    // }
  }

  void bufferize() {
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(),
                 vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices_.size(),
                 indices_.data(), GL_STATIC_DRAW);

    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, TexCoords));
    glBindVertexArray(0);

    bufferized_ = true;
  }

 private:
  uint VAO_ = 0, VBO_ = 0, EBO_ = 0;
  bool bufferized_ = false;
  std::vector<Vertex> vertices_;
  std::vector<unsigned> indices_;
  std::vector<TextureComponent> textures_;
};
}  // namespace s21
#endif  // MY_MESH
