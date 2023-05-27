#include "Scene.h"
#include "Utils.h"
#include "events/KeyPressedEvent.h"
#include "events/KeyReleasedEvent.h"
#include "events/MouseDoubleClickedEvent.h"
#include "events/MouseMovedEvent.h"
#include "events/MouseScrolledEvent.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

void Scene::MousePos(QPoint pos) {
  static auto& input = scene_.GetComponent<Input>(Utils::GetInput(&scene_));

  input.mouse_pos = pos;
}

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

void Scene::MousePressed(QPoint pos) {
  qDebug() << "MousePressedEvent:" << pos;
  static auto& input =
      scene_.GetComponent<Input>(scene_.GetEntities<Input>().at(0));
  input.left_button_pressed = true;
  input.start = pos;
  input.end = pos;
}

void Scene::MouseReleased(QPoint pos) {
  qDebug() << "MouseReleasedEvent:" << pos;
  static auto& input =
      scene_.GetComponent<Input>(scene_.GetEntities<Input>().at(0));
  input.left_button_pressed = false;
  input.start = pos;
  input.end = pos;
}

void Scene::MouseMoved(QPoint pos) {
  qDebug() << "MouseMovedEvent:" << pos;
  // MouseMovedEvent event(offset);
  // scene_.Despatch(event);

  static auto& input =
      scene_.GetComponent<Input>(scene_.GetEntities<Input>().at(0));
  input.end = pos;
}

void Scene::MouseDoubleClicked(QPoint pos) {
  qDebug() << "MouseDoubleClicked:" << pos;
  // MouseDoubleClickedEvent event(pos);
  // scene_.Despatch(event);

  static auto& input =
      scene_.GetComponent<Input>(scene_.GetEntities<Input>().at(0));
  input.double_click = pos;
  picked_ = true;
}

void Scene::KeyPressed(QKeyEvent* key_event) {
  qDebug() << "KeyPressedEvent:" << key_event;
  // KeyPressedEvent event(key_event);
  // scene_.Despatch(event);

  static auto& input =
      scene_.GetComponent<Input>(scene_.GetEntities<Input>().at(0));
  input.keys[key_event->key()] = true;
}

void Scene::KeyReleased(QKeyEvent* key_event) {
  qDebug() << "KeyReleasedEvent" << key_event;

  static auto& input =
      scene_.GetComponent<Input>(scene_.GetEntities<Input>().at(0));
  input.keys[key_event->key()] = false;
  // KeyReleasedEvent event(key_event);
  // scene_.Despatch(event);
}

void Scene::MouseScrolled(float scroll) {
  qDebug() << "MouseScrolledEvent" << scroll;
  // MouseScrolledEvent event(scroll);
  // scene_.Despatch(event);

  static auto& camera = scene_.GetComponent<Camera>(Utils::GetCamera(&scene_));
  camera.zoom += scroll;
}
}  // namespace s21
