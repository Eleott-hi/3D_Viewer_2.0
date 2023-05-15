#include "Backend.h"
#include "events/KeyPressedEvent.h"
#include "events/KeyReleasedEvent.h"
#include "events/MouseDoubleClickedEvent.h"
#include "events/MouseMovedEvent.h"
#include "events/MouseScrolledEvent.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

void Backend::WindowResize(int w, int h) {
  qDebug() << "WindowResizeEvent:" << w << h;
  WindowResizeEvent event(w, h);
  scene_.Despatch(event);

  width_ = w;
  height_ = h;

  framebuffer3D_->Resize(w, h);
  g_buffer_->Resize(w, h);
}

void Backend::MousePressed(QPoint pos) {
  qDebug() << "MousePressedEvent:" << pos;
  static auto& mouse =
      scene_.GetComponent<MouseInput>(scene_.GetEntities<MouseInput>().at(0));
  mouse.left_button_pressed = true;
  mouse.start = pos;
  mouse.end = pos;
}

void Backend::MouseReleased(QPoint pos) {
  qDebug() << "MouseReleasedEvent:" << pos;
  static auto& mouse =
      scene_.GetComponent<MouseInput>(scene_.GetEntities<MouseInput>().at(0));
  mouse.left_button_pressed = false;
  mouse.start = pos;
  mouse.end = pos;
}

void Backend::MouseMoved(QPoint pos) {
  qDebug() << "MouseMovedEvent:" << pos;
  // MouseMovedEvent event(offset);
  // scene_.Despatch(event);

  static auto& mouse =
      scene_.GetComponent<MouseInput>(scene_.GetEntities<MouseInput>().at(0));
  mouse.end = pos;
}

void Backend::MouseDoubleClicked(QPoint pos) {
  qDebug() << "MouseDoubleClicked:" << pos;
  // MouseDoubleClickedEvent event(pos);
  // scene_.Despatch(event);

  static auto& mouse =
      scene_.GetComponent<MouseInput>(scene_.GetEntities<MouseInput>().at(0));
  mouse.double_click = pos;
  picked_ = true;
}

void Backend::KeyPressed(QKeyEvent* key_event) {
  qDebug() << "KeyPressedEvent:" << key_event;
  // KeyPressedEvent event(key_event);
  // scene_.Despatch(event);

  static auto& keyboard = scene_.GetComponent<KeyboardInput>(
      scene_.GetEntities<KeyboardInput>().at(0));
  keyboard.keys[key_event->key()] = true;
}

void Backend::KeyReleased(QKeyEvent* key_event) {
  qDebug() << "KeyReleasedEvent" << key_event;

  static auto& keyboard = scene_.GetComponent<KeyboardInput>(
      scene_.GetEntities<KeyboardInput>().at(0));
  keyboard.keys[key_event->key()] = false;
  // KeyReleasedEvent event(key_event);
  // scene_.Despatch(event);
}

void Backend::MouseScrolled(float scroll) {
  qDebug() << "MouseScrolledEvent" << scroll;
  MouseScrolledEvent event(scroll);
  scene_.Despatch(event);
}
}  // namespace s21
