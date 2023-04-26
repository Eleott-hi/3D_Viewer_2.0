
#pragma once

#include <QPoint>
#include <QVector3D>

#include "components/Components.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

enum class CameraDirection { NONE, FORWARD, BACKWARD, LEFT, RIGHT };

class CameraSystem : public System {
 public:
  CameraSystem() = default;
  ~CameraSystem() = default;

  void Init(std::shared_ptr<ECS_Controller> const &scene);
  void OnWindowResize(Event &event);
  void UpdateCameraInfo(TransformComponent const &transform);
  void SetPerspective(bool value) { perspective_ = value; }
  void Update(float deltaTime, const QPoint &offset,
              CameraDirection direction = CameraDirection::NONE);

 private:
  bool perspective_ = true;
  QVector3D front_, right_, up_;
  std::shared_ptr<ECS_Controller> scene_;
  QMatrix4x4 perspectiveMatrix_, orthoMatrix_;

  void ProcessMouseMovement(QVector3D &Rotation,   //
                            const QPoint &offset,  //
                            bool constrainPitch = true);

  void UpdatePosition(QVector3D &Position,  //
                      float deltaTime,      //
                      CameraDirection direction);

  void UpdateLookAtVectors(QVector3D const &rotation);

  void UpdateViewMatrix(CameraComponent &camera,  //
                        const QVector3D &Position);
};
}  // namespace s21
