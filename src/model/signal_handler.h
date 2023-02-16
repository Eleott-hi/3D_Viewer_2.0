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
  void SetStackCamera();
  void SetObjectsCount(const QString &);

  // ============================ OBJECT SIGNALS =============================
  void SetStackObject();
  void SetCameraPanel(QVector3D position, QVector3D rotation, float zoom);
  void SetInfoPanel(QString const &file, uint32_t vertices, uint32_t indices);
  void SetTransformPanel(QVector3D position, QVector3D rotation, float scale);
  void SetPointPanel(bool show, bool smooth, int size, QColor color);
  void SetLinePanel(bool show, bool dashed, int size, QColor color);
  void SetTexturePanel(int index);
  void SetMaterialPanel(float shininess);
  void SetPointLightPanel(bool sourceLight, QVector3D const &ambient,
                          QVector3D const &diffuse, QVector3D const &specular,
                          QVector3D const &constants);
  void SetAxisPanel(bool value);

 private:
  SignalHandler() = default;
  ~SignalHandler() = default;
  SignalHandler(SignalHandler &&) = delete;
  SignalHandler(const SignalHandler &) = delete;
  SignalHandler &operator=(SignalHandler &&) = delete;
  SignalHandler &operator=(const SignalHandler &) = delete;
};

}  // namespace s21
