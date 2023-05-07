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
}

void Backend::MouseMoved(QPoint offset) {
  qDebug() << "MouseMovedEvent:" << offset;
  MouseMovedEvent event(offset);
  scene_.Despatch(event);
}

void Backend::MouseDoubleClicked(QPoint pos) {
  qDebug() << "MouseDoubleClicked:" << pos;
  MouseDoubleClickedEvent event(pos);
  scene_.Despatch(event);
}

void Backend::KeyPressed(QKeyEvent* key_event) {
  qDebug() << "KeyPressedEvent:" << key_event;
  KeyPressedEvent event(key_event);
  scene_.Despatch(event);
}

void Backend::KeyReleased(QKeyEvent* key_event) {
  qDebug() << "KeyReleasedEvent" << key_event;
  KeyReleasedEvent event(key_event);
  scene_.Despatch(event);
}

void Backend::MouseScrolled(float scroll) {
  qDebug() << "MouseScrolledEvent" << scroll;
  MouseScrolledEvent event(scroll);
  scene_.Despatch(event);
}
}  // namespace s21
