#pragma once

#include "backend.h"

namespace s21 {

class Controller : public QObject {
  Q_OBJECT

 public:
  ~Controller() = default;
  Controller(const Controller &c) = delete;
  void operator=(const Controller &c) = delete;

  explicit Controller(Backend *model) : backend_(model) {}

  void AddModel(const std::string &filename) { backend_->AddModel(filename); }

  void UpdateCameraInfo(const QVector3D &position,  //
                        const QVector3D &rotation,  //
                        float zoom) {
    backend_->UpdateCameraInfo(position, rotation, zoom);
  }

  void SetPerspective(bool value) { backend_->SetPerspective(value); }

  void UpdateTransformInfo(const QVector3D &position,  //
                           const QVector3D &rotation,  //
                           float scale) {
    backend_->UpdateTransformInfo(position, rotation, scale);
  }

  void UpdatePointInfo(bool show, bool smooth, int size, QColor const &color) {
    backend_->UpdatePointInfo(show, smooth, size, color);
  }

  void UpdateLineInfo(bool show, bool dashed, int size, QColor const &color) {
    backend_->UpdateLineInfo(show, dashed, size, color);
  }

  void UpdateAxisInfo(bool value) { backend_->UpdateAxisInfo(value); }

  void UpdateTextureTechnique(int index) {
    backend_->UpdateTextureTechnique(index);
  }

  void LoadTexture(std::string const &filename) {
    backend_->LoadTexture(filename);
  }

  void UpdateMaterialInfo(float value) { backend_->UpdateMaterialInfo(value); }

  void MakeLightSource(bool value) { backend_->MakeLightSource(value); }

  void UpdatePointLightInfo(QVector3D const &ambient,   //
                            QVector3D const &diffuse,   //
                            QVector3D const &specular,  //
                            QVector3D const &constants) {
    backend_->UpdatePointLightInfo(ambient, diffuse, specular, constants);
  }

  void DeleteObject() { backend_->DeleteObject(); }

  // =========================== Render =============================
  void Init() { backend_->Init(); }
  void render() { backend_->Render(); }
  void resize(uint32_t w, uint32_t h) { backend_->Resize(w, h); }

 private:
  Backend *backend_;
};
}  // namespace s21
