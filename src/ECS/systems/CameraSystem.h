
#pragma once

#include <QPoint>

#include "WindowResizeEvent.h"
#include "components/Components.h"
#include "core/Controller.h"
#include "core/System.h"

namespace s21 {

enum class CameraDirection { NONE, FORWARD, BACKWARD, LEFT, RIGHT };

class CameraSystem : public System {
 public:
  CameraSystem() = default;
  ~CameraSystem() = default;

  void Init(std::shared_ptr<Controller> const &scene);
  void OnWindowResize(Event &event);
  void UpdateCameraInfo(TransformComponent const &transform);
  void SetPerspective(bool value) { perspective_ = value; }
  void Update(float deltaTime,       //
              const QPoint &offset,  //
              CameraDirection direction = CameraDirection::NONE);

 private:
  std::shared_ptr<Controller> scene_;
  QMatrix4x4 perspectiveMatrix_, orthoMatrix_;
  bool perspective_ = true;

  void processMouseMovement(QVector3D &Rotation,   //
                            const QPoint &offset,  //
                            bool constrainPitch = true);
  void UpdatePosition(QVector3D &Position,     //
                      const QVector3D &Front,  //
                      const QVector3D &Right,  //
                      float deltaTime,         //
                      CameraDirection direction);
  void UpdateLookAtVectors(const TransformComponent &transform,
                           QVector3D &Front,  //
                           QVector3D &Right,  //
                           QVector3D &Up);
};
}  // namespace s21
