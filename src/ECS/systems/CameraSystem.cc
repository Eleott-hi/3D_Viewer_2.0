#include "CameraSystem.h"

#include "Utils.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

void CameraSystem::Init(std::shared_ptr<ECS_Controller> const &scene) {
  scene_ = scene;
  scene_->AddEventListener(EventType::WindowResize,
                           BIND_EVENT_FN(OnWindowResize));
  orthoMatrix_.ortho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
}

void CameraSystem::OnWindowResize(Event &event) {
  auto &cast_event = static_cast<WindowResizeEvent &>(event);

  QMatrix4x4 m;
  m.perspective(80.0,
                (float)cast_event.GetWidth() / (float)cast_event.GetHeight(),
                1.0, 1000.0);

  perspectiveMatrix_ = std::move(m);
}

void CameraSystem::UpdateCameraInfo(TransformComponent const &component) {
  for (auto &entity : entities_) {
    auto &transform = scene_->GetComponent<TransformComponent>(entity);
    transform = component;
  }
}

void CameraSystem::Update(float deltaTime,       //
                          const QPoint &offset,  //
                          CameraDirection direction) {
  for (auto &entity : entities_) {
    auto &camera = scene_->GetComponent<CameraComponent>(entity);
    auto &transform = scene_->GetComponent<TransformComponent>(entity);

    camera.projectionMatrix = perspective_ ? perspectiveMatrix_ : orthoMatrix_;

    ProcessMouseMovement(transform.rotation, offset);
    UpdateLookAtVectors(transform.rotation);
    UpdatePosition(transform.position, deltaTime, direction);
    UpdateViewMatrix(camera, transform.position);
  }
}

void CameraSystem::ProcessMouseMovement(QVector3D &rotation,
                                        QPoint const &offset,
                                        bool constrainPitch) {
  // TODO (pintoved): Fish-Eye problem
  const float MouseSensitivity = 0.1;

  float Yaw = rotation.y() + offset.x() * MouseSensitivity;
  float Pitch = rotation.x() - offset.y() * MouseSensitivity;

  if (constrainPitch) Pitch = qBound(-89.0f, Pitch, 89.0f);

  Utils::SetVector(rotation, Pitch, Yaw, 0);
}

void CameraSystem::UpdatePosition(QVector3D &position,  //
                                  float deltaTime,      //
                                  CameraDirection direction) {
  const float speed = 0.1;
  float velocity = speed * deltaTime;
  switch (direction) {
    case CameraDirection::FORWARD:
      position += front_ * velocity;
      break;
    case CameraDirection::BACKWARD:
      position -= front_ * velocity;
      break;
    case CameraDirection::LEFT:
      position -= right_ * velocity;
      break;
    case CameraDirection::RIGHT:
      position += right_ * velocity;
      break;
    default:
      break;
  }
}

void CameraSystem::UpdateLookAtVectors(QVector3D const &rotation) {
  QVector3D WorldUp = {0, 1, 0};
  float pitch = qDegreesToRadians(rotation.x());
  float yaw = qDegreesToRadians(rotation.y());

  Utils::SetVector(front_, cos(yaw) * cos(pitch),  //
                   sin(pitch),                     //
                   sin(yaw) * cos(pitch));
  front_.normalize();

  right_ = QVector3D::normal(front_, WorldUp);
  up_ = QVector3D::normal(right_, front_);
}

void CameraSystem::UpdateViewMatrix(CameraComponent &camera,
                                    const QVector3D &position) {
  QMatrix4x4 tmp;
  tmp.lookAt(position, position + front_, up_);
  camera.viewMatrix = std::move(tmp);
}

}  // namespace s21
