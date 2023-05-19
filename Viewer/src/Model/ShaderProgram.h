#pragma once

#include <QOpenGLShaderProgram>
#include <QString>

namespace s21 {
class ShaderProgram {
 public:
  ShaderProgram() = default;
  ~ShaderProgram() = default;

  template <typename Type>
  void setUniformValue(QString name, Type value) {
    shader_.setUniformValue(name.toStdString().c_str(), value);
  }

  bool bind() { return shader_.bind(); }

  void GenerateShaders(QString vertex_file, QString fragment_file);

 private:
  QOpenGLShaderProgram shader_;
};

}  // namespace s21
