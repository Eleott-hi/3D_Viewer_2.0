#ifndef SRC_HEADERS_AXIS_H
#define SRC_HEADERS_AXIS_H

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

#include "technique_strategy.h"

namespace s21 {
class Axis : protected QOpenGLExtraFunctions {
 private:
  uint VAO_[2], VBO_[2];
  bool bufferized_;

 public:
  ~Axis() = default;
  Axis(Axis &&) = default;
  Axis(const Axis &) = default;
  Axis &operator=(Axis &&) = default;
  Axis &operator=(const Axis &) = default;

  Axis() { initializeOpenGLFunctions(); }

  void Draw(TechniqueStrategy *technique) {
    if (!bufferized_) bufferize();

    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_POINT_SMOOTH);
    glPointSize(10.0);

    for (int i = 0; i < 3; ++i) {
      if (i == 0)
        technique->setColor(Qt::red);
      else if (i == 1)
        technique->setColor(Qt::green);
      else if (i == 2)
        technique->setColor(Qt::blue);

      glBindVertexArray(VAO_[0]);
      glDrawArrays(GL_LINES, i * 2, 2);

      glBindVertexArray(VAO_[1]);
      glDrawArrays(GL_POINTS, i, 1);
    }

    glBindVertexArray(0);
  }

 private:
  void bufferize() {
    // ================================ CUBE ================================
    float lines[] = {
        -1000000, 0,        0,        1000000, 0,       0,         // x-axis
        0,        -1000000, 0,        0,       1000000, 0,         // y-axis
        0,        0,        -1000000, 0,       0,       1000000};  // z-axis

    float arrows[] = {1, 0, 0,   // x-arrow
                      0, 1, 0,   // y-arrow
                      0, 0, 1};  // z-arrow
    // ================================ CUBE ================================

    glGenVertexArrays(2, VAO_);
    glGenBuffers(2, VBO_);

    glBindVertexArray(VAO_[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO_[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrows), arrows, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    bufferized_ = true;
  }
};

}  // namespace s21

#endif  // SRC_HEADERS_AXIS_H
