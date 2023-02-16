#ifndef SRC_HEADERS_CAMERA_H
#define SRC_HEADERS_CAMERA_H

#include <QMatrix4x4>
#include <QPoint>
#include <QVector3D>
#include <vector>

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

  const QVector3D &getPosition() const { return Position; }
  const QVector3D &getFront() const { return Front; }
  const QMatrix4x4 &getViewMatrix() const { return viewMatrix_; }
  void Reset();
  void Move(float deltaTime, CameraDirection direction = CameraDirection::NONE);
  void processMouseMovement(QPoint offset, bool constrainPitch = true);
  void processMouseScroll(float yoffset);

 private:
  QMatrix4x4 viewMatrix_;
  // =========================== DEBUG DELETE ===========================
  QVector3D Position = {14.0843, 7.39728, -14.4024};

  float Yaw = 120.4, Pitch = -13.6;

  //  float Yaw = -90.0, Pitch = 0.0;
  //  QVector3D Position = {0, 0, 3};
  // =========================== DEBUG DELETE ===========================

  // //  camera Attributes
  QVector3D  // Position = {0, 0, 3},
      WorldUp = {0, 1, 0},
      Front, Up, Right;

  float speed = 0.1, MouseSensitivity = 0.1, Zoom = 45.0;

  //   //  euler Angles
  //  float Yaw = -90.0, Pitch = 0.0;

  void UpdateCameraVectors();
  void updateViewMatrix();
};
}  // namespace s21

#endif  // SRC_HEADERS_CAMERA_H
