#include "Scene.h"
#include "Utils.h"
// #include "events/KeyPressedEvent.h"
// #include "events/KeyReleasedEvent.h"
// #include "events/MouseDoubleClickedEvent.h"
// #include "events/MouseMovedEvent.h"
// #include "events/MouseScrolledEvent.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

void Scene::WindowResize(int w, int h) {
  qDebug() << "WindowResizeEvent:" << w << h;
  WindowResizeEvent event(w, h);
  scene_.Despatch(event);

  width_ = w, height_ = h;

  g_buffer_->Resize(w, h);
  framebuffer3D_->Resize(w, h);
  framebufferShadow_->Resize(w, h);
  defaultFramebuffer_->Resize(w, h, false);

  auto& camera = scene_.GetComponent<Camera>(Utils::GetCamera(&scene_));
  camera.width = w, camera.height = h;
}

void Scene::MousePos(QPoint pos) {}
void Scene::MousePressed(QPoint pos) {}
void Scene::MouseReleased(QPoint pos) {}
void Scene::MouseMoved(QPoint pos) {}
void Scene::MouseDoubleClicked(QPoint pos) {}
void Scene::KeyPressed(QKeyEvent* key_event) {}
void Scene::KeyReleased(QKeyEvent* key_event) {}
void Scene::MouseScrolled(float scroll) {}
}  // namespace s21
