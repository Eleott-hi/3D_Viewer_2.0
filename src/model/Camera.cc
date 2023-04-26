#include "Camera.h"

namespace s21 {

Camera::Camera() { UpdateCameraVectors(); }

void Camera::Reset() {
  yaw_ = -90.0;
  pitch_ = 0.0;
  position_ = {0, 0, 3};
  UpdateCameraVectors();
}

void Camera::ProcessKeyboard(float deltaTime, CameraDirection direction) {
  float velocity = speed_ * deltaTime;

  switch (direction) {
    case CameraDirection::FORWARD:
      position_ += front_ * velocity;
      break;
    case CameraDirection::BACKWARD:
      position_ -= front_ * velocity;
      break;
    case CameraDirection::LEFT:
      position_ -= right_ * velocity;
      break;
    case CameraDirection::RIGHT:
      position_ += right_ * velocity;
      break;
    default:
      break;
  }

  updateViewMatrix();
}

void Camera::ProcessMouseMovement(QPoint offset, bool constrainPitch) {
  // Fish eye problem
  yaw_ += offset.x() * mouse_sensitivity_;
  pitch_ -= offset.y() * mouse_sensitivity_;

  if (constrainPitch) pitch_ = qBound(-89.0, pitch_, 89.0);

  UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
  zoom_ = qBound(1.0, zoom_ - yoffset, 45.0);
}

void Camera::UpdateCameraVectors() {
  float yaw = qDegreesToRadians(yaw_);
  float pitch = qDegreesToRadians(pitch_);

  front_.setX(cos(yaw) * cos(pitch));
  front_.setY(sin(pitch));
  front_.setZ(sin(yaw) * cos(pitch));
  front_.normalize();

  right_ = QVector3D::normal(front_, world_up_);
  up_ = QVector3D::normal(right_, front_);

  updateViewMatrix();
}

void Camera::updateViewMatrix() {
  QMatrix4x4 tmp;
  tmp.lookAt(position_, position_ + front_, up_);
  viewMatrix_ = std::move(tmp);
}

}  // namespace s21
