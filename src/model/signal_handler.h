#pragma once

#include <QObject>

namespace s21 {

class SignalHandler : public QObject {
  Q_OBJECT

 public:
  static SignalHandler &GetInstance() {
    static SignalHandler instance;
    return instance;
  }

 signals:
  // ============================ CAMERA SIGNALS =============================
  void SetObjectsCount(const QString &);
  void SetStackCamera();

  // ============================ OBJECT SIGNALS =============================
  void SetInfoPanel(QString const &file, uint32_t vertices, uint32_t indices);
  void SetTransformPanel(QVector3D position, QVector3D rotation, float scale);
  void SetCameraPanel(QVector3D position, QVector3D rotation, float zoom);
  void SetPointPanel(bool show, bool smooth, int size, QColor color);
  void SetLinePanel(bool show, bool dashed, int size, QColor color);
  void SetMaterialPanel(float shininess);
  void SetTexturePanel(int index);
  void SetAxisPanel(bool value);
  void SetStackObject();

  void SetPointLightPanel(bool sourceLight, QVector3D const &ambient,
                          QVector3D const &diffuse, QVector3D const &specular,
                          QVector3D const &constants);

 private:
  SignalHandler() = default;
  ~SignalHandler() = default;
  SignalHandler(SignalHandler &&) = delete;
  SignalHandler(const SignalHandler &) = delete;
  SignalHandler &operator=(SignalHandler &&) = delete;
  SignalHandler &operator=(const SignalHandler &) = delete;
};

}  // namespace s21
