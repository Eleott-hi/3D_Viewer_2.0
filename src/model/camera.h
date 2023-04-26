#ifndef SRC_HEADERS_CAMERA_H
#define SRC_HEADERS_CAMERA_H

#include <QMatrix4x4>
#include <QPoint>
#include <QVector3D>

namespace s21 {

enum class CameraDirection { NONE, FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
 public:
  Camera();
  ~Camera() = default;
  Camera(Camera &&) = default;
  Camera(const Camera &) = default;
  Camera &operator=(Camera &&) = default;
  Camera &operator=(const Camera &) = default;

  const QVector3D &GetFront() const { return front_; }
  const QVector3D &GetPosition() const { return position_; }
  const QMatrix4x4 &GetViewMatrix() const { return viewMatrix_; }

  void Reset();
  void ProcessMouseScroll(float yoffset);
  void ProcessMouseMovement(QPoint offset, bool constrainPitch = true);
  void ProcessKeyboard(float deltaTime,
                       CameraDirection direction = CameraDirection::NONE);

 private:
  QMatrix4x4 viewMatrix_;
  QVector3D position_ = {14.0843, 7.39728, -14.4024};
  QVector3D world_up_ = {0, 1, 0};
  QVector3D front_, up_, right_;

  float yaw_ = 120.4, pitch_ = -13.6;
  float speed_ = 0.1, mouse_sensitivity_ = 0.1, zoom_ = 45.0;

  void UpdateCameraVectors();
  void updateViewMatrix();
};
}  // namespace s21

#endif  // SRC_HEADERS_CAMERA_H
        // //  camera Attributes
        // QVector3D  // Position = {0, 0, 3},

//   //  euler Angles
//  float Yaw = -90.0, Pitch = 0.0;
