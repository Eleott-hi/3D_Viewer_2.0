#ifndef SRC_HEADERS_BACKEND_H
#define SRC_HEADERS_BACKEND_H

#include <QObject>
#include <QOpenGLWidget>
#include <QString>
#include <functional>
#include <memory>

#include "Components.h"
#include "Parser.h"
#include "TextureStorage.h"
#include "core/ECS_Controller.h"
#include "systems/CameraSystem.h"
#include "systems/CubemapSystem.h"
#include "systems/DefferedShadingSystem.h"
#include "systems/EditPickedSystem.h"
#include "systems/InputSystem.h"
#include "systems/LightSystem.h"
#include "systems/MousePickingSystem.h"
#include "systems/ProjectionSystem.h"
#include "systems/Render2DSystem.h"
#include "systems/RenderPickedSystem.h"
#include "systems/RenderSystem.h"
#include "technique_strategy.h"

namespace s21 {

class Backend : QOpenGLExtraFunctions {
 public:
  Backend() = default;
  ~Backend() = default;
  Backend(Backend &&) = delete;
  Backend(Backend const &) = delete;
  Backend &operator=(Backend &&) = delete;
  Backend &operator=(Backend const &) = delete;

  void Init(QOpenGLWidget *widget);
  void Render();
  void AddModel(QString path);
  void LoadTexture(QString filename);

  void MouseMoved(QPoint offset);
  void MouseScrolled(float scroll);
  void MouseDoubleClicked(QPoint pos);
  void KeyPressed(QKeyEvent *key_event);
  void KeyReleased(QKeyEvent *key_event);
  void WindowResize(int width, int height);
  void MousePressed(QPoint pos);
  void MouseReleased(QPoint pos);

  template <typename Type>
  void UpdateComponent(Type const &component) {
    editPickedSystem_->UpdateComponent(component);
  }

  template <typename Type>
  Type *GetComponent() {
    return editPickedSystem_->GetComponent<Type>();
  }

  template <typename Type>
  void ChangeComponent(std::function<void(Type &)> update) {
    auto component = editPickedSystem_->GetComponent<Type>();

    if (component) {
      Q_ASSERT(update);
      update(*component);
    }

    opengl_widget_->update();
  }

 private:
  ECS_Controller scene_;
  std::shared_ptr<Parser> parser_;
  QOpenGLWidget *opengl_widget_ = nullptr;
  std::shared_ptr<TechniqueStrategy> technique_;
  std::shared_ptr<TextureStorage> texture_storage_;
  std::shared_ptr<IFramebuffer> g_buffer_;
  std::shared_ptr<IFramebuffer> framebuffer3D_;

  std::shared_ptr<InputSystem> inputSystem_;
  std::shared_ptr<LightSystem> lightSystem_;
  std::shared_ptr<CameraSystem> cameraSystem_;
  std::shared_ptr<RenderSystem> renderSystem_;
  std::shared_ptr<CubemapSystem> cubemapSystem_;
  std::shared_ptr<Render2DSystem> render2DSystem_;
  std::shared_ptr<EditPickedSystem> editPickedSystem_;
  std::shared_ptr<ProjectionSystem> projectionSystem_;
  std::shared_ptr<MousePickingSystem> mousePickingSystem_;
  std::shared_ptr<RenderPickedSystem> renderPickedSystem_;
  std::shared_ptr<DefferedShadingSystem> defferedShadingSystem_;

  void RegisterComponents();
  void RegisterSystems();
  void Draw();
  void Update();
  void PrepareFramebuffer3D();
};

}  // namespace s21

#endif  // SRC_HEADERS_BACKEND_H
