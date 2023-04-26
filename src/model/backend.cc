#include "backend.h"

#include "components/Components.h"
#include "events/WindowResizeEvent.h"
#include "stb_image.h"

namespace s21 {

// ============================ OBJECTS =============================

void Backend::AddLight() {}

void Backend::AddModel(const std::string &filename) {
  const auto &model = modelParser_->loadModel(filename);
  static int tmp = 0;  // Debug

  if (model.first) {
    EntityID entity = scene_->NewEntity();
    scene_->AddComponent<MeshComponent>(entity, model.second);
    scene_->AddComponent<TextureComponent>(entity);
    scene_->AddComponent<MaterialComponent>(entity);
    scene_->AddComponent<TechniqueComponent>(entity);
    scene_->AddComponent<TransformComponent>(entity);
    scene_->AddComponent<LineSettingsComponent>(
        entity,
        (tmp++ == 0)                                              // Debug
            ? LineSettingsComponent{false, false, 1, Qt::yellow}  // Debug
            : LineSettingsComponent{}                             // Debug
    );
    scene_->AddComponent<PointSettingsComponent>(entity);
    scene_->AddComponent<LightSettingsComponent>(entity);
  }

  uint32_t count = scene_->GetEntities<MeshComponent>().size();
  emit signal_handler_.SetObjectsCount(QString::number(count));
}

void Backend::UpdateCameraInfo(TransformComponent const &component) {
  cameraSystem_->UpdateCameraInfo(component);
}

void Backend::SetPerspective(bool value) {
  cameraSystem_->SetPerspective(value);
}

// ======================= Object Update ============================

void Backend::UpdateTransformInfo(TransformComponent const &component) {
  objectEditorSystem_->UpdateTransformInfo(component);
}

void Backend::UpdatePointInfo(PointSettingsComponent const &component) {
  objectEditorSystem_->UpdatePointInfo(component);
}

void Backend::UpdateLineInfo(LineSettingsComponent const &component) {
  objectEditorSystem_->UpdateLineInfo(component);
}

void Backend::UpdateAxisInfo(bool value) {
  objectEditorSystem_->UpdateAxisInfo(value);
}

void Backend::UpdateTextureTechnique(int index) {
  objectEditorSystem_->UpdateTextureTechnique((TechniqueType)index);
}

void Backend::LoadTexture(std::string const &filename) {
  auto textureID = textureStorage_->LoadTexture(filename);

  if (textureID)
    objectEditorSystem_->UpdateTextureInfo(textureID, TextureType::DIFFUSE);
}

void Backend::UpdateMaterialInfo(float value) {  //
  objectEditorSystem_->UpdateMaterialInfo({value});
}

void Backend::MakeLightSource(bool value) {
  objectEditorSystem_->MakeLightSource(value);
}

void Backend::UpdatePointLightInfo(LightSettingsComponent const &component) {
  objectEditorSystem_->UpdatePointLightInfo(component);
}

void Backend::DeleteObject() {
  objectEditorSystem_->DeleteObject();

  uint32_t count = scene_->GetEntities<MeshComponent>().size();
  emit signal_handler_.SetObjectsCount(QString::number(count));
  emit signal_handler_.SetStackCamera();
}

// ==============+========== OpenGL SURFACE =========================
void Backend::Init() {
  textureStorage_ = std::make_shared<TextureStorage>();
  modelParser_ = std::make_shared<ModelParser>(textureStorage_);
  technique_ = std::make_shared<TechniqueStrategy>();
  scene_ = std::make_shared<ECS_Controller>();

  glEnable(GL_DEPTH_TEST);
  glLineStipple(4, 0xAAAA);

  RegisterComponents();
  RegisterSystems();

  camera_ = scene_->NewEntity();
  scene_->AddComponent<CameraComponent>(camera_);
  scene_->AddComponent<TransformComponent>(camera_);

  // ======================== DEBUG ==========================
  auto &tmp = scene_->GetComponent<TransformComponent>(camera_);
  tmp.position = {14.0843, 7.39728, -14.4024};
  tmp.rotation = {-13.6, 120.4, 0};
  // ======================== DEBUG ==========================
}

void Backend::Resize(uint32_t width, uint32_t height) {
  WindowResizeEvent event(width, height);
  scene_->Despatch(event);
}

void Backend::Render() {
  Update();
  Draw();
}

void Backend::Update() {
  cameraSystem_->Update(1, cSettings_.Offset(), cSettings_.MoveCamera());
  cSettings_.Offset(QPoint{0, 0});
  cSettings_.MoveCamera(CameraDirection::NONE);

  if (cSettings_.MousePicking()) {
    cSettings_.MousePicking(false);
    const QPoint &mousePosition = cSettings_.GetMousePosition();
    mousePickingSystem_->Update(camera_, mousePosition)
        ? emit signal_handler_.SetStackObject()
        : emit signal_handler_.SetStackCamera();
    objectEditorSystem_->ObjectSelected();
  }
}

void Backend::Draw() {
  Clear();
  renderSystem_->Update(camera_);
  renderPickedSystem_->Update(camera_);
}

void Backend::Clear() {
  auto const &c = cSettings_.getBackground();
  glClearColor(c.redF(), c.greenF(), c.blueF(), c.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Backend::RegisterComponents() {
  scene_->RegisterComponent<MeshComponent>();
  scene_->RegisterComponent<AxisComponent>();
  scene_->RegisterComponent<CameraComponent>();
  scene_->RegisterComponent<TextureComponent>();
  scene_->RegisterComponent<PickingComponent>();
  scene_->RegisterComponent<MaterialComponent>();
  scene_->RegisterComponent<TechniqueComponent>();
  scene_->RegisterComponent<TransformComponent>();
  scene_->RegisterComponent<PointLightComponent>();
  scene_->RegisterComponent<LineSettingsComponent>();
  scene_->RegisterComponent<PointSettingsComponent>();
  scene_->RegisterComponent<LightSettingsComponent>();
}

void Backend::RegisterSystems() {
  cameraSystem_ = scene_->RegisterSystem<CameraSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<CameraComponent>());
    mask.set(GetComponentID<TransformComponent>());
    scene_->ChangeSystemMask<CameraSystem>(mask);
    cameraSystem_->Init(scene_);
  }

  objectEditorSystem_ = scene_->RegisterSystem<ObjectEditorSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<PickingComponent>());
    scene_->ChangeSystemMask<ObjectEditorSystem>(mask);
    objectEditorSystem_->Init(scene_);
  }

  mousePickingSystem_ = scene_->RegisterSystem<MousePickingSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<MeshComponent>());
    mask.set(GetComponentID<TransformComponent>());
    scene_->ChangeSystemMask<MousePickingSystem>(mask);
    mousePickingSystem_->Init(scene_, technique_);
  }

  renderSystem_ = scene_->RegisterSystem<RenderSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<MeshComponent>());
    mask.set(GetComponentID<TransformComponent>());
    scene_->ChangeSystemMask<RenderSystem>(mask);
    renderSystem_->Init(scene_, technique_);
  }

  renderPickedSystem_ = scene_->RegisterSystem<RenderPickedSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<PickingComponent>());
    scene_->ChangeSystemMask<RenderPickedSystem>(mask);
    renderPickedSystem_->Init(scene_, technique_);
  }
}

}  // namespace s21
