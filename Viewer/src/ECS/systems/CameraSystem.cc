#include "CameraSystem.h"

#include "Utils.h"
#include "events/KeyPressedEvent.h"
#include "events/KeyReleasedEvent.h"
#include "events/MouseMovedEvent.h"
#include "events/MouseScrolledEvent.h"

namespace s21 {

void CameraSystem::Init(ECS_Controller *scene) { scene_ = scene; }

void CameraSystem::Update() {
  for (auto &entity : entities_) {
    auto &camera = scene_->GetComponent<Camera>(entity);
    UpdateLookAtVectors(camera);
  }
}

void CameraSystem::UpdateLookAtVectors(Camera &camera) {
  static const QVector3D world_up = {0, 1, 0};

  const auto yaw = qDegreesToRadians(camera.yaw);
  const auto pitch = qDegreesToRadians(camera.pitch);

  camera.front.setX(cos(yaw) * cos(pitch));
  camera.front.setY(sin(pitch));
  camera.front.setZ(sin(yaw) * cos(pitch));
  camera.front.normalize();

  camera.right = QVector3D::normal(camera.front, world_up);
  camera.up = QVector3D::normal(camera.right, camera.front);
}

}  // namespace s21
