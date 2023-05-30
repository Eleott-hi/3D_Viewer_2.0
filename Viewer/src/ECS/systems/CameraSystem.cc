#include "CameraSystem.h"

#include "Input.h"
#include "Utils.h"

namespace s21 {

void UpdateLookAtVectors(Camera &camera) {
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

void ProcessKeyboardInput(Camera &camera, Timer &timer) {
  const float velocity = camera.speed * timer.time;

  if (Input::IsKeyPressed(Qt::Key_W))
    camera.position += camera.front * velocity;
  if (Input::IsKeyPressed(Qt::Key_S))
    camera.position -= camera.front * velocity;
  if (Input::IsKeyPressed(Qt::Key_D))
    camera.position += camera.right * velocity;
  if (Input::IsKeyPressed(Qt::Key_A))
    camera.position -= camera.right * velocity;
  if (Input::IsKeyPressed(Qt::Key_R))  //
    camera.position += camera.up * velocity;
  if (Input::IsKeyPressed(Qt::Key_F))  //
    camera.position -= camera.up * velocity;
}

void ProcessMouseInput(Camera &camera, Timer &timer) {
  if (Input::IsMouseButtonPressed(Qt::LeftButton)) {
    QPoint offset = Input::MouseLastPosition() - Input::MousePosition();

    camera.yaw += offset.x() * camera.mouse_sensitivity;
    camera.pitch -= offset.y() * camera.mouse_sensitivity;

    if (camera.constrain_pitch)
      camera.pitch = qBound(-89.0f, camera.pitch, 89.0f);

    UpdateLookAtVectors(camera);
  }
}

void UpdateMatricies(Camera &camera) {
  QMatrix4x4 view;
  QMatrix4x4 projection;

  camera.perspective
      ? projection.perspective(camera.zoom,
                               (float)camera.width / (float)camera.height,
                               camera.near_clip, camera.far_clip)
      : projection.ortho(camera.left_clip, camera.right_clip,
                         camera.bottom_clip, camera.top_clip, camera.near_clip,
                         camera.far_clip);

  camera.projection_matrix = projection;

  view.lookAt(camera.position, camera.position + camera.front, camera.up);
  camera.view_matrix = view;
}

void CameraSystem::Init(ECS_Controller *scene) { scene_ = scene; }

void CameraSystem::Update() {
  auto &timer = scene_->GetComponent<Timer>(Utils::GetTimer(scene_));

  if (Input::IsKeyPressed(Qt::Key_Control)) return;
  // if (scene_->GetEntities<PickingTag>().size()) return;

  for (auto &entity : entities_) {
    auto &camera = scene_->GetComponent<Camera>(entity);

    ProcessMouseInput(camera, timer);
    ProcessKeyboardInput(camera, timer);
    UpdateMatricies(camera);
  }
}

}  // namespace s21
