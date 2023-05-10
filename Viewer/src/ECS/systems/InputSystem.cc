#include "InputSystem.h"

#include "Utils.h"

namespace s21 {

void InputSystem::Update() {
  for (auto entity : entities_) {
    if (scene_->EntityHasComponent<KeyboardInput>(entity))
      ProcessKeyPressed(scene_->GetComponent<KeyboardInput>(entity));

    if (scene_->EntityHasComponent<MouseInput>(entity))
      ProcessMouseMovement(scene_->GetComponent<MouseInput>(entity));
  }
}

void InputSystem::ProcessKeyPressed(KeyboardInput &input) {
  static const float speed = 0.1;
  static const auto deltaTime = 1;
  static const float velocity = speed * deltaTime;
  static auto &camera =
      scene_->GetComponent<Camera>(Utils::GetCameraID(scene_));

  auto &[keys] = input;

  if (keys[Qt::Key_W]) camera.position += camera.front * velocity;
  if (keys[Qt::Key_S]) camera.position -= camera.front * velocity;
  if (keys[Qt::Key_D]) camera.position += camera.right * velocity;
  if (keys[Qt::Key_A]) camera.position -= camera.right * velocity;
  if (keys[Qt::Key_R]) camera.position += camera.up * velocity;
  if (keys[Qt::Key_F]) camera.position -= camera.up * velocity;
}

void InputSystem::ProcessMouseMovement(MouseInput &input, bool constrainPitch) {
  static const float MouseSensitivity = 0.08;
  static auto &camera =
      scene_->GetComponent<Camera>(Utils::GetCameraID(scene_));


  if (input.left_button_pressed) {
    auto offset = input.start - input.end;
    input.start = input.end;

    camera.yaw += offset.x() * MouseSensitivity;
    camera.pitch -= offset.y() * MouseSensitivity;

    if (constrainPitch) camera.pitch = qBound(-89.0f, camera.pitch, 89.0f);
  }
}

}  // namespace s21
