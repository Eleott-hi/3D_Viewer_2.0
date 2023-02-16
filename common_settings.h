#ifndef SRC_HEADERS_COMMON_SETTING_H
#define SRC_HEADERS_COMMON_SETTING_H

#include <QColor>
#include <QMatrix4x4>
#include <QSettings>
#include <QVector3D>

#include "CameraSystem.h"

namespace s21 {

class CommonSettings : public QObject {
  Q_OBJECT
 public:
  CommonSettings(CommonSettings &&) = delete;
  CommonSettings(const CommonSettings &) = delete;
  CommonSettings &operator=(CommonSettings &&) = delete;
  CommonSettings &operator=(const CommonSettings &) = delete;

  static CommonSettings &GetInstance() {
    static CommonSettings instance;
    return instance;
  }

  // ======================== Setters ========================
  void setWidgetSize(unsigned w, unsigned h) { width_ = w, height_ = h; }
  void SetMousePosition(const QPoint &value) { mousePosition_ = value; }
  void SetBackColor(const QColor &value) { background_ = value; }
  void MoveCamera(CameraDirection value) { direction_ = value; }
  void SetPickedObject(bool value) { objectIsPicked_ = value; }
  void Perspective(bool value) { perspective_ = value; }
  void Offset(const QPoint &value) { offset_ = value; }
  void MousePicking(bool value) { picking_ = value; }

  // ======================== Getters ========================
  const QPoint &GetMousePosition() const { return mousePosition_; }
  const QColor &getBackground() const { return background_; }
  bool ObjectIsPicked() const { return objectIsPicked_; }
  CameraDirection MoveCamera() { return direction_; }
  bool MousePicking() const { return picking_; }
  bool Perspective() { return perspective_; }
  const QPoint &Offset() { return offset_; }
  int getHeight() const { return height_; }
  int getWidth() const { return width_; }

  // ======================== Private ========================
 private:
  CommonSettings() { loadSettings(); }
  ~CommonSettings() { saveSettings(); }

  void saveSettings() { qDebug() << "NEEDS SAVE SETTINGS"; }
  void loadSettings() { qDebug() << "NEEDS LOAD SETTINGS"; }

 private:
  std::unique_ptr<QSettings> settingsFile_ =
      std::make_unique<QSettings>("pintovedTeam", "3D_Viewer");

 private:
  QColor background_;
  QPoint mousePosition_;
  QPoint offset_;
  unsigned width_, height_;
  
  CameraDirection direction_ = CameraDirection::NONE;

  bool ctrl = false;
  bool perspective_ = true;
  bool objectIsPicked_ = false, picking_ = false;
};

}  // namespace s21

#endif  // SRC_HEADERS_COMMON_SETTING_H
