
#pragma once

#include <QPoint>

#include "Components.h"
#include "ECS_Controller.h"
#include "System.h"
#include "WindowResizeEvent.h"

namespace s21 {

enum class CameraDirection { NONE, FORWARD, BACKWARD, LEFT, RIGHT };

class CameraSystem : public System {
public:
  CameraSystem() = default;
  ~CameraSystem() = default;

  void Init(std::shared_ptr<Controller> const &scene) {
    scene_ = scene;
    scene_->AddEventListener(EventType::WindowResize,
                             BIND_EVENT_FN(OnWindowResize));
    orthoMatrix_.ortho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
  }

  void OnWindowResize(Event &event) {
    auto &cast_event = static_cast<WindowResizeEvent &>(event);

    QMatrix4x4 m;
    m.perspective(80.0,
                  (float)cast_event.GetWidth() / (float)cast_event.GetHeight(),
                  1.0, 1000.0);

    perspectiveMatrix_ = std::move(m);
  }

  void UpdateCameraInfo(TransformComponent const &transform) {
    for (auto &entity : entities_) {
      qDebug() << "HERE";
      auto &cameraTransform = scene_->GetComponent<TransformComponent>(entity);
      cameraTransform = transform;
    }
  }

  void SetPerspective(bool value) { perspective_ = value; }

  void Update(float deltaTime, const QPoint &offset,
              CameraDirection direction = CameraDirection::NONE) { //

    for (auto &entity : entities_) {
      auto &camera = scene_->GetComponent<CameraComponent>(entity);
      auto &transform = scene_->GetComponent<TransformComponent>(entity);

      camera.projectionMatrix =
          perspective_ ? perspectiveMatrix_ : orthoMatrix_;

      QVector3D front, right, up;
      processMouseMovement(transform.rotation, offset);
      UpdateLookAtVectors(transform, front, right, up);
      UpdatePosition(transform.position, front, right, deltaTime, direction);
      updateViewMatrix(camera, transform.position, front, up);
    }
  }

private:
  std::shared_ptr<Controller> scene_;
  QMatrix4x4 perspectiveMatrix_, orthoMatrix_;
  bool perspective_ = true;

  void processMouseMovement(QVector3D &Rotation, const QPoint &offset,
                            bool constrainPitch = true) {
    // TODO (pintoved): Fish-Eye problem
    const float MouseSensitivity = 0.1;

    float Pitch = Rotation.x();
    float Yaw = Rotation.y();
    Yaw += offset.x() * MouseSensitivity;
    Pitch -= offset.y() * MouseSensitivity;
    if (constrainPitch) {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }
    Rotation = {Pitch, Yaw, 0};
  }

  void UpdatePosition(QVector3D &Position, const QVector3D &Front,
                      const QVector3D &Right, float deltaTime,
                      CameraDirection direction) {
    const float speed = 0.1;
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
  }

  void UpdateLookAtVectors(const TransformComponent &transform,
                           QVector3D &Front, QVector3D &Right, QVector3D &Up) {
    QVector3D WorldUp = {0, 1, 0};
    float Pitch = transform.rotation.x();
    float Yaw = transform.rotation.y();

    Front =
        QVector3D(
            cos(qDegreesToRadians(Yaw)) * cos(qDegreesToRadians(Pitch)), // 0
            sin(qDegreesToRadians(Pitch)),                               // 0
            sin(qDegreesToRadians(Yaw)) * cos(qDegreesToRadians(Pitch))) // -1
            .normalized();
    Right = QVector3D::crossProduct(Front, WorldUp).normalized();
    Up = QVector3D::crossProduct(Right, Front).normalized();
  }

  void updateViewMatrix(CameraComponent &camera, const QVector3D &Position,
                        const QVector3D &Front, const QVector3D &Up) {
    QMatrix4x4 tmp;
    tmp.lookAt(Position, Position + Front, Up);
    camera.viewMatrix = std::move(tmp);
  }
};
} // namespace s21
