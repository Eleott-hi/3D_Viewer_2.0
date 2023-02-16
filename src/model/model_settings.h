#ifndef SRC_HEADERS_MODEL_SETTINGS_H
#define SRC_HEADERS_MODEL_SETTINGS_H

#include <QColor>
#include <QMatrix4x4>
#include <QString>
#include <QVector3D>

#include "signal_handler.h"

namespace s21 {

class ModelSettings {
 private:
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();
  QMatrix4x4 previosRotationMatrix_, currentRotationMatrix_;
  QMatrix4x4 normalizeMatrix_;
  QVector3D position_, rotation_;
  QColor vertexColor_ = {Qt::green}, lineColor_;
  QString filename_;
  uint vertices_ = 0, indices_ = 0;
  bool axisLines_ = true, pointSmooth_ = false, dashedLines_ = false,
       show_vertex_ = true, normalize_ = false, fillFaces_ = true;
  float scale_ = 1, lineWidth_ = 1, pointSize_ = 20, shininess_ = 32;

 public:
  ModelSettings() = default;
  ~ModelSettings() = default;
  ModelSettings(ModelSettings &&) = default;
  ModelSettings(const ModelSettings &) = default;
  ModelSettings &operator=(ModelSettings &&) = default;
  ModelSettings &operator=(const ModelSettings &) = default;



  void AddVerticies(uint count) { vertices_ += count; }
  void AddIndices(uint count) { indices_ += count; }

  // ======================== Setter ========================
  void setNormalizeMatrix(const QMatrix4x4 &m) { normalizeMatrix_ = m; }
  void setVertexColor(const QColor &value) { vertexColor_ = value; }
  void Translate(const QVector3D &value) { position_ += value; }
  void SetFilename(const QString &value) { filename_ = value; }
  void Rotate(const QVector3D &value) { position_ += value; }
  void setLinesForm(bool value) { dashedLines_ = value; }
  void setPointForm(bool value) { pointSmooth_ = value; }
  void setLineWidth(float value) { lineWidth_ = value; }
  void setShininess(float value) { shininess_ = value; }
  void setPointSize(float value) { pointSize_ = value; }
  void setFillFaces(bool value) { fillFaces_ = value; }
  void setNormalize(bool value) { normalize_ = value; }
  void SetAxis(bool value) { axisLines_ = value; }
  void Scale(float value) { scale_ = value; }
  // ======================== Setter ========================

  // ======================== Getter ========================
  QMatrix4x4 getModelMatrix() {
    return translationMatrix() * scaleMatrix() * rotationMatrix() *
           (normalize_ ? normalizeMatrix_ : QMatrix4x4());
  }
  QMatrix4x4 getAxisMatrix() { return translationMatrix() * rotationMatrix(); }
  const QColor &getVertexColor() const { return vertexColor_; }
  const QVector3D &getPosition() const { return position_; }
  const QString &GetFilename() const { return filename_; }
  bool getPointForm() const { return pointSmooth_; }
  bool getLinesForm() const { return dashedLines_; }
  float getPointSize() const { return pointSize_; }
  float getLineWidth() const { return lineWidth_; }
  float getShininess() const { return shininess_; }
  bool getFillFaces() const { return fillFaces_; }
  bool getAxis() const { return axisLines_; }
  float GetScale() { return scale_; }
  // ======================== Getter ========================

 private:
  QMatrix4x4 scaleMatrix() const {
    QMatrix4x4 m;
    m.scale(scale_);
    return m;
  }

  QMatrix4x4 rotationMatrix() {
    QMatrix4x4 m;
    //    m.rotate(0, rotation_); //  quaternion
    m.rotate(rotation_.x(), 1, 0, 0);
    m.rotate(rotation_.y(), 0, 1, 0);
    m.rotate(rotation_.z(), 0, 0, 1);
    currentRotationMatrix_ = m * previosRotationMatrix_;
    return currentRotationMatrix_;
  }

  QMatrix4x4 translationMatrix() const {
    QMatrix4x4 m;
    m.translate(position_);
    return m;
  }
};

}  // namespace s21

#endif  // SRC_HEADERS_MODEL_SETTINGS_H
