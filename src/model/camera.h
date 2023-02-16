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
  ~Camera() = default;
  Camera(Camera &&) = default;
  Camera(const Camera &) = default;
  Camera &operator=(Camera &&) = default;
  Camera &operator=(const Camera &) = default;

  Camera() { UpdateCameraVectors(); }

  const QVector3D &getPosition() const { return Position; }
  const QVector3D &getFront() const { return Front; }

  void Reset() {
    Yaw = -90.0, Pitch = 0.0;
    Position = {0, 0, 3};
    UpdateCameraVectors();
  }

  const QMatrix4x4 &getViewMatrix() const { return viewMatrix_; }

  void Move(float deltaTime,
            CameraDirection direction = CameraDirection::NONE) {
    float velocity = speed * deltaTime;
    switch (direction) {
      case CameraDirection::FORWARD:
        Position += Front * velocity;
        break;
      case CameraDirection::BACKWARD:
        Position -= Front * velocity;
        break;
      case CameraDirection::LEFT:
        Position -= Right * velocity;
        break;
      case CameraDirection::RIGHT:
        Position += Right * velocity;
        break;
      default:
        break;
    }
    updateViewMatrix();
  }

  void processMouseMovement(QPoint offset, bool constrainPitch = true) {
    // Fish eye problem
    Yaw += offset.x() * MouseSensitivity;
    Pitch -= offset.y() * MouseSensitivity;
    if (constrainPitch) {
      if (Pitch > 89.0f) Pitch = 89.0f;
      if (Pitch < -89.0f) Pitch = -89.0f;
    }
    UpdateCameraVectors();
  }

  void processMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f) Zoom = 1.0f;
    if (Zoom > 45.0f) Zoom = 45.0f;
  }

 private:
  void UpdateCameraVectors() {
    Front =
        QVector3D(
            cos(qDegreesToRadians(Yaw)) * cos(qDegreesToRadians(Pitch)),  // 0
            sin(qDegreesToRadians(Pitch)),                                // 0
            sin(qDegreesToRadians(Yaw)) * cos(qDegreesToRadians(Pitch)))  // -1
            .normalized();
    Right = QVector3D::crossProduct(Front, WorldUp).normalized();
    Up = QVector3D::crossProduct(Right, Front).normalized();

    updateViewMatrix();
  }

  void updateViewMatrix() {
    QMatrix4x4 tmp;
    tmp.lookAt(Position, Position + Front, Up);
    viewMatrix_ = std::move(tmp);
  }

 private:
  QMatrix4x4 viewMatrix_;
  // =========================== DEBUG DELETE ===========================
  QVector3D Position = {14.0843, 7.39728, -14.4024};
  float Yaw = 120.4;
  float Pitch = -13.6;

  //  float Yaw = -90.0, Pitch = 0.0;
  //  QVector3D Position = {0, 0, 3};
  // =========================== DEBUG DELETE ===========================

  //  // camera Attributes
  QVector3D  // Position = {0, 0, 3},
      WorldUp = {0, 1, 0},
      Front, Up, Right;

  float speed = 0.1, MouseSensitivity = 0.1, Zoom = 45.0;

  //   //  euler Angles
  //  float Yaw = -90.0, Pitch = 0.0;
};
}  // namespace s21

#endif  // SRC_HEADERS_CAMERA_H
