#include "Camera.h"

namespace s21 {

Camera::Camera() { UpdateCameraVectors(); }

void Camera::Reset() {
  Yaw = -90.0, Pitch = 0.0;
  Position = {0, 0, 3};
  UpdateCameraVectors();
}

void Camera::Move(float deltaTime, CameraDirection direction) {
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

void Camera::processMouseMovement(QPoint offset, bool constrainPitch) {
  // Fish eye problem
  Yaw += offset.x() * MouseSensitivity;
  Pitch -= offset.y() * MouseSensitivity;
  if (constrainPitch) {
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
  }
  UpdateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
  Zoom -= (float)yoffset;
  if (Zoom < 1.0f) Zoom = 1.0f;
  if (Zoom > 45.0f) Zoom = 45.0f;
}

void Camera::UpdateCameraVectors() {
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

void Camera::updateViewMatrix() {
  QMatrix4x4 tmp;
  tmp.lookAt(Position, Position + Front, Up);
  viewMatrix_ = std::move(tmp);
}

}  // namespace s21
