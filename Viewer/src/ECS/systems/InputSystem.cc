#include "InputSystem.h"

#include "Utils.h"

namespace s21 {

void InputSystem::Update() {
  for (auto entity : entities_) {
    auto &input = scene_->GetComponent<Input>(entity);
    ProcessMouseMovement(input);
    ProcessKeyPressed(input);
  }
}

void InputSystem::ProcessKeyPressed(Input &input) {
  static auto &camera =
      scene_->GetComponent<Camera>(Utils::GetCameraID(scene_));
  static auto const &timer =
      scene_->GetComponent<Timer>(Utils::GetTickTime(scene_));

  const float speed = 0.004;
  const float velocity = speed * timer.time;

  // static const float speed = 0.1;
  // static const auto deltaTime = 1;
  // static const float velocity = speed * deltaTime;

  // qDebug() << "Velocity: " << velocity;

  auto &keys = input.keys;

  if (keys[Qt::Key_W]) camera.position += camera.front * velocity;
  if (keys[Qt::Key_S]) camera.position -= camera.front * velocity;
  if (keys[Qt::Key_D]) camera.position += camera.right * velocity;
  if (keys[Qt::Key_A]) camera.position -= camera.right * velocity;
  if (keys[Qt::Key_R]) camera.position += camera.up * velocity;
  if (keys[Qt::Key_F]) camera.position -= camera.up * velocity;
}

void InputSystem::ProcessMouseMovement(Input &input, bool constrainPitch) {
  static auto &camera =
      scene_->GetComponent<Camera>(Utils::GetCameraID(scene_));
  static auto const &timer =
      scene_->GetComponent<Timer>(Utils::GetTickTime(scene_));

  const float speed = 0.1;
  const float MouseSensitivity = 0.1;//= speed * timer.time;

  // qDebug() << "MouseSensitivity: " << MouseSensitivity;

  // static const float MouseSensitivity = 0.08;

  if (input.left_button_pressed) {
    auto offset = input.start - input.end;
    input.start = input.end;

    qDebug() << offset;

    camera.yaw += offset.x() * MouseSensitivity;
    camera.pitch -= offset.y() * MouseSensitivity;
  }

  if (constrainPitch) camera.pitch = qBound(-89.0f, camera.pitch, 89.0f);
  camera.Update();
}

}  // namespace s21
