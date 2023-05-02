
#pragma once

#include <QKeyEvent>
#include <QPoint>
#include <QVector3D>
#include <unordered_map>

#include "Components.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

enum class CameraDirection { NONE, FORWARD, BACKWARD, LEFT, RIGHT };

class CameraSystem : public System {
 public:
  CameraSystem() = default;
  ~CameraSystem() = default;

  void UpdateCameraInfo(Camera const &component);
  void Update();
  void Init(ECS_Controller *scene);

 private:
  ECS_Controller *scene_ = nullptr;
  std::unordered_map<int, bool> keys_pressed_ = {{Qt::Key_W, false},
                                                 {Qt::Key_S, false},
                                                 {Qt::Key_A, false},
                                                 {Qt::Key_D, false}};

  void ProcessMouseMovement(Camera &camera, QPoint offset,
                            bool constrainPitch = true);

  void UpdateLookAtVectors(Camera &camera);

  void ProcessKeyPressed(Camera &camera, QKeyEvent *key_event);

  void OnMouseMoved(Event &e);
  void OnKeyPressed(Event &e);
  void OnKeyReleased(Event &e);
};
}  // namespace s21
