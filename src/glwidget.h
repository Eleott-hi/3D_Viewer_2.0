#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>

#include "Mesh.h"
#include "shader.h"

extern "C" {
#include "file_parcer.h"
#include "matrix_algebra/s21_matrix.h"
}

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 private:
  bool perspective = 1, pointSmooth = 0, dashedLines = 0, axisLines = 0;
  bool ctrl = 0, show_vertex = 1;
  float xRot = 0, yRot = 0, zRot = 0;
  float xTrans = 0, yTrans = 0, zTrans = -2;
  float xScale = 1, yScale = 1, zScale = 1;

  float bRed = 0, bGreen = 0, bBlue = 0;
  float mRed = 1, mGreen = 1, mBlue = 1;

  float lineWidth = 1, pointSize = 1;
  float height = 0, width = 0;
  Mesh modelData;
  matrix_t matrixIdentity, matrixNormalized, matrixRotationCurrent,
      matrixRotationPrevios;
  QPoint mPos;
  Shader *shader;

 public:
  GLWidget(QWidget *parent = nullptr);
  ~GLWidget();
  void setColor(bool background, float r, float g, float b);
  void setPerspective(bool value) { perspective = value; }
  void setAxisLines(bool value) { axisLines = value; }
  void setPointForm(bool value) { pointSmooth = value; }
  void setLinesForm(bool value) { dashedLines = value; }
  void setCtrl(bool value) { ctrl = value; }
  void setPointSize(float value) { pointSize = value; }
  void setLineWidth(float value) { lineWidth = value; }
  void setXTrans(float value) { xTrans = value; }
  void setYTrans(float value) { yTrans = value; }
  void setZTrans(float value) { zTrans = value; }
  void setXRot(float value) { xRot = value; }
  void setYRot(float value) { yRot = value; }
  void setZRot(float value) { zRot = value; }
  void setXScale(float value) { xScale = value; }
  void setYScale(float value) { yScale = value; }
  void setZScale(float value) { zScale = value; }
  void initRotationMatrix();
  void setdots(bool vertex) { show_vertex = vertex; }
  void remember_rotation_matrix();
  void setModelData(const char *filename);
  unsigned getVertexCount() { return modelData.getVerteces().size() / 3 - 1; }
  unsigned getFacetCount() { return modelData.getFacets().size(); }
  float **getRotationMatrix() { return matrixRotationPrevios.matrix; }

 protected:
  // QOpenGLWidget interface
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  // QWidget interface
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

 private:
  matrix_t setRotation(float x, float y, float z);
  matrix_t _setRotation(float angle, int x, int y, int z);
  matrix_t setTranslation(float d_x, float d_y, float d_z);
  matrix_t setScale(float x, float y, float z);
  matrix_t setScale(float x);
  matrix_t setPerspective(float l, float r, float b, float t, float n, float f);
  matrix_t setOrtho(float l, float r, float b, float t, float n, float f);
  matrix_t setIdentityMatrix();
  void refreshSettings(bool needNormalize);
  void copyMatrix(matrix_t *from, matrix_t *to);
  void setNormalizedMatrix();
  void drawCoordinates();
  void drawModel();
};

#endif  // GLWIDGET_H
