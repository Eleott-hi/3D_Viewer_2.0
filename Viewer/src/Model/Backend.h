#ifndef SRC_HEADERS_BACKEND_H
#define SRC_HEADERS_BACKEND_H

#include <QObject>
#include <QString>
#include <memory>

#include "Components.h"
#include "Parser.h"
#include "core/ECS_Controller.h"
#include "systems/CameraSystem.h"
#include "systems/LightSystem.h"
#include "systems/MousePickingSystem.h"
#include "systems/ProjectionSystem.h"
#include "systems/RenderPickedSystem.h"
#include "systems/RenderSystem.h"
#include "systems/Render2DSystem.h"
#include "technique_strategy.h"

namespace s21 {

class Backend
//        :private QObject
{
  //  Q_OBJECT
 public:
  Backend() = default;
  ~Backend() = default;
  Backend(Backend &&) = delete;
  Backend(Backend const &) = delete;
  Backend &operator=(Backend &&) = delete;
  Backend &operator=(Backend const &) = delete;

  void Init();
  void Render();
  void AddModel(QString path);

  void MouseMoved(QPoint offset);
  void MouseDoubleClicked(QPoint pos);
  void WindowResize(int width, int height);
  void KeyPressed(QKeyEvent *key_event);
  void KeyReleased(QKeyEvent *key_event);

 private:
  Parser parser_;
  ECS_Controller scene_;
  std::shared_ptr<Render2DSystem>   render2DSystem_;
  std::shared_ptr<LightSystem> lightSystem_;
  std::shared_ptr<CameraSystem> cameraSystem_;
  std::shared_ptr<RenderSystem> renderSystem_;
  std::shared_ptr<ProjectionSystem> projectionSystem_;
  std::shared_ptr<MousePickingSystem> mousePickingSystem_;
  std::shared_ptr<RenderPickedSystem> renderPickedSystem_;
  std::shared_ptr<TechniqueStrategy> technique_;

  void RegisterComponents();
  void RegisterSystems();
  void Draw();
  void Update();
  void Clear();
};

}  // namespace s21

#endif  // SRC_HEADERS_BACKEND_H
