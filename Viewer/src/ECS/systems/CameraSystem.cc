#include "CameraSystem.h"

#include "Utils.h"
#include "events/KeyPressedEvent.h"
#include "events/KeyReleasedEvent.h"
#include "events/MouseMovedEvent.h"

namespace s21 {

void CameraSystem::Init(ECS_Controller *scene) {
  scene_ = scene;
  scene_->AddEventListener(EventType::MouseMoved, BIND_EVENT_FN(OnMouseMoved));
  scene_->AddEventListener(EventType::KeyPressed, BIND_EVENT_FN(OnKeyPressed));
  scene_->AddEventListener(EventType::KeyReleased,
                           BIND_EVENT_FN(OnKeyReleased));
}

void CameraSystem::OnMouseMoved(Event &e) {
  auto &event = static_cast<MouseMovedEvent &>(e);
  for (auto &entity : entities_) {
    auto &camera = scene_->GetComponent<Camera>(entity);
    ProcessMouseMovement(camera, event.Offset());
    UpdateLookAtVectors(camera);
  }
}

void CameraSystem::OnKeyPressed(Event &e) {
  auto &event = static_cast<KeyPressedEvent &>(e);

  keys_pressed_[event.Key()->key()] = true;

  // for (auto &entity : entities_) {
  //   auto &camera = scene_->GetComponent<Camera>(entity);
  //   ProcessKeyPressed(camera, event.Key());
  //   UpdateLookAtVectors(camera);
  // }
}

void CameraSystem::OnKeyReleased(Event &e) {
  auto &event = static_cast<KeyReleasedEvent &>(e);

  keys_pressed_[event.Key()->key()] = false;
}

void CameraSystem::UpdateCameraInfo(Camera const &component) {
  for (auto &entity : entities_) {
    auto &camera = scene_->GetComponent<Camera>(entity);
    camera = component;
    UpdateLookAtVectors(camera);
  }
}

void CameraSystem::Update() {
  for (auto &entity : entities_) {
    auto &camera = scene_->GetComponent<Camera>(entity);

    //    UpdateLookAtVectors(camera);
    ProcessKeyPressed(camera, nullptr);
  }
}

void CameraSystem::ProcessMouseMovement(Camera &camera, QPoint offset,
                                        bool constrainPitch) {
  static const float MouseSensitivity = 0.1;

  camera.yaw += offset.x() * MouseSensitivity;
  camera.pitch -= offset.y() * MouseSensitivity;

  if (constrainPitch) camera.pitch = qBound(-89.0f, camera.pitch, 89.0f);
}

void CameraSystem::ProcessKeyPressed(Camera &camera, QKeyEvent *key_event) {
  static const float speed = 0.1;
  static const auto deltaTime = 1;
  static const float velocity = speed * deltaTime;

  if (keys_pressed_[Qt::Key_W]) camera.position += camera.front * velocity;
  if (keys_pressed_[Qt::Key_S]) camera.position -= camera.front * velocity;
  if (keys_pressed_[Qt::Key_A]) camera.position -= camera.right * velocity;
  if (keys_pressed_[Qt::Key_D]) camera.position += camera.right * velocity;

  // switch (key_event->key()) {
  //   case Qt::Key_W:
  //     break;
  //   case Qt::Key_S:
  //     camera.position -= camera.front * velocity;
  //     break;
  //   case Qt::Key_A:
  //     camera.position -= camera.right * velocity;
  //     break;
  //   case Qt::Key_D:
  //     camera.position += camera.right * velocity;
  //     break;
  //   default:
  //     break;
  // }
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
