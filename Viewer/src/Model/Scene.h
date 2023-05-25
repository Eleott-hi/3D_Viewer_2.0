#ifndef SRC_HEADERS_BACKEND_H
#define SRC_HEADERS_BACKEND_H

#include <QObject>
#include <QOpenGLWidget>
#include <QString>
#include <functional>
#include <memory>

#include "Components.h"
#include "Observable.h"
#include "Parser.h"
#include "TextureStorage.h"
#include "core/ECS_Controller.h"
#include "systems/CameraSystem.h"
#include "systems/CubemapSystem.h"
#include "systems/DefferedShadingSystem.h"
#include "systems/EditPickedSystem.h"
#include "systems/GizmoRenderSystem.h"
#include "systems/InputSystem.h"
#include "systems/LightSystem.h"
#include "systems/MousePickingSystem.h"
#include "systems/PointShadowRenderSystem.h"
#include "systems/PointShadowSystem.h"
#include "systems/Render2DSystem.h"
#include "systems/RenderPickedSystem.h"
#include "systems/RenderSystem.h"
#include "systems/ShadowRenderSystem.h"
#include "systems/ShadowSystem.h"
#include "systems/TimeTickSystem.h"
#include "technique_strategy.h"

namespace s21 {

class Scene : public Observable, protected QOpenGLExtraFunctions {
 public:
  Scene() = default;
  ~Scene() = default;
  Scene(Scene &&) = delete;
  Scene(Scene const &) = delete;
  Scene &operator=(Scene &&) = delete;
  Scene &operator=(Scene const &) = delete;

  void Render();
  void AddModel(QString path);
  void Init(QOpenGLWidget *widget);
  void LoadTexture(QString filename, Texture &texture);

  void MouseMoved(QPoint offset);
  void MouseScrolled(float scroll);
  void MouseDoubleClicked(QPoint pos);
  void KeyPressed(QKeyEvent *key_event);
  void KeyReleased(QKeyEvent *key_event);
  void WindowResize(int width, int height);
  void MousePressed(QPoint pos);
  void MouseReleased(QPoint pos);
  void MousePos(QPoint pos);

  template <typename Type>
  void UpdateComponent(Type const &component) {
    editPickedSystem_->UpdateComponent(component);
  }

  template <typename Type>
  std::vector<Type *> GetComponents() {
    return editPickedSystem_->GetComponents<Type>();
  }

  template <typename Type>
  void ChangeComponent(std::function<void(Type &)> update) {
    auto components = GetComponents<Type>();

    Q_ASSERT(update);
    for (auto component : components) update(*component);

    opengl_widget_->update();
  }

  template <typename Type>
  void RemoveComponent() {
    editPickedSystem_->RemoveComponent<Type>();
  }

  template <typename Type>
  void AddComponent() {
    editPickedSystem_->AddComponent<Type>();
  }

  Camera &GetCamera();

  ECS_Controller *GetScene() { return &scene_; }

 private:
  bool picked_ = false;
  uint32_t width_ = 500, height_ = 500;
  ECS_Controller scene_;
  QOpenGLWidget *opengl_widget_ = nullptr;
  std::shared_ptr<Parser> parser_;
  std::shared_ptr<TechniqueStrategy> technique_;
  std::shared_ptr<TextureStorage> texture_storage_;

  std::shared_ptr<IFramebuffer> g_buffer_;
  std::shared_ptr<IFramebuffer> framebuffer3D_;
  std::shared_ptr<IFramebuffer> framebufferShadow_;
  std::shared_ptr<IFramebuffer> defaultFramebuffer_;
  std::shared_ptr<IFramebuffer> pointShadowFramebuffer_;
  // std::shared_ptr<IFramebuffer> mousePickingFramebuffer_;

  std::shared_ptr<InputSystem> inputSystem_;
  std::shared_ptr<LightSystem> lightSystem_;
  std::shared_ptr<ShadowSystem> shadowSystem_;
  std::shared_ptr<CameraSystem> cameraSystem_;
  std::shared_ptr<RenderSystem> renderSystem_;
  std::shared_ptr<CubemapSystem> cubemapSystem_;
  std::shared_ptr<TimeTickSystem> timeTickSystem_;
  std::shared_ptr<Render2DSystem> render2DSystem_;
  std::shared_ptr<EditPickedSystem> editPickedSystem_;
  std::shared_ptr<PointShadowSystem> pointShadowSystem_;
  std::shared_ptr<MousePickingSystem> mousePickingSystem_;
  std::shared_ptr<RenderPickedSystem> renderPickedSystem_;
  std::shared_ptr<ShadowRenderSystem> shadowRenderSystem_;
  std::shared_ptr<DefferedShadingSystem> defferedShadingSystem_;
  std::shared_ptr<PointShadowRenderSystem> pointShadowRenderSystem_;
  std::shared_ptr<GizmoRenderSystem> gizmoRenderSystem_;

  void RegisterComponents();
  void RegisterSystems();
  void Draw();
  void Update();
  void DebugLights(bool directional, bool point_1, bool point_2, bool spot);
  void InsideOpenGLContext(std::function<void()> func);
  void SetFramebuffers();
  void InitEntities();
};

}  // namespace s21

#endif  // SRC_HEADERS_BACKEND_H
