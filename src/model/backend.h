#ifndef SRC_HEADERS_BACKEND_H
#define SRC_HEADERS_BACKEND_H

#include <memory>

#include "ModelParser.h"
#include "TextureStorage.h"
#include "common_settings.h"
#include "core/ECS_Controller.h"
#include "signal_handler.h"
#include "systems/CameraSystem.h"
#include "systems/MousePickingSystem.h"
#include "systems/ObjectEditorSystem.h"
#include "systems/RenderPickedSystem.h"
#include "systems/RenderSystem.h"

namespace s21 {
class Backend {
 public:
  Backend() = default;
  ~Backend() = default;
  Backend(Backend &&) = delete;
  Backend(Backend const &) = delete;
  Backend &operator=(Backend &&) = delete;
  Backend &operator=(Backend const &) = delete;

  // ============================ OBJECTS =============================
  void AddLight();
  void AddModel(std::string const &filename);
  void UpdateCameraInfo(TransformComponent const &component);

  void SetPerspective(bool value);

  void UpdateTransformInfo(TransformComponent const &component);
  void UpdatePointInfo(PointSettingsComponent const &component);
  void UpdateLineInfo(LineSettingsComponent const &component);
  void UpdateAxisInfo(bool value);
  void UpdateTextureTechnique(int index);
  void LoadTexture(std::string const &filename);
  void UpdateMaterialInfo(float value);
  void DeleteObject();

  // ========================= Light Settings =========================
  void MakeLightSource(bool value);
  void UpdatePointLightInfo(LightComponent const &component);

  // ==============+========== OpenGL SURFACE =========================
  void Init();
  void Resize(uint32_t width, uint32_t height);
  void Render();

 private:
  EntityID camera_;
  std::shared_ptr<ECS_Controller> scene_;
  std::shared_ptr<ModelParser> modelParser_;
  std::shared_ptr<TechniqueStrategy> technique_;
  std::shared_ptr<TextureStorage> textureStorage_;

  std::shared_ptr<CameraSystem> cameraSystem_;
  std::shared_ptr<RenderSystem> renderSystem_;
  std::shared_ptr<RenderPickedSystem> renderPickedSystem_;
  std::shared_ptr<MousePickingSystem> mousePickingSystem_;
  std::shared_ptr<ObjectEditorSystem> objectEditorSystem_;

  CommonSettings &cSettings_ = CommonSettings::GetInstance();
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();

  void RegisterComponents();
  void RegisterSystems();
  void Update();
  void Draw();
  void Clear();
};
}  // namespace s21

#endif  // SRC_HEADERS_BACKEND_H
