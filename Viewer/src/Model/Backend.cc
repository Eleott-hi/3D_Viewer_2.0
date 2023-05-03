#include "Backend.h"

#include "events/KeyPressedEvent.h"
#include "events/KeyReleasedEvent.h"
#include "events/MouseDoubleClickedEvent.h"
#include "events/MouseMovedEvent.h"
#include "events/MouseScrolledEvent.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

void Backend::Init() {
  technique_ = std::make_shared<TechniqueStrategy>();

  glLineStipple(4, 0xAAAA);

  RegisterComponents();
  RegisterSystems();

  EntityID camera = scene_.NewEntity();
  scene_.AddComponent<Camera>(camera);

  EntityID projection = scene_.NewEntity();
  scene_.AddComponent<Projection>(projection);

  EntityID light = scene_.NewEntity();
  scene_.AddComponent<Light>(light);
  scene_.AddComponent<DirectionalLight>(light);
}

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

void Backend::Render() {
  Update();
  Draw();
}

void Backend::AddModel(QString path) {
  auto model = parser_.loadModel(path);

  if (model) {
    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Model>(entity, std::move(model.value()));
    // scene_->AddComponent<LightComponent>(entity);
    // scene_->AddComponent<TextureComponent>(entity);
    scene_.AddComponent<Material>(entity);
    // scene_->AddComponent<TechniqueComponent>(entity);
    scene_.AddComponent<Transform>(entity);
    // scene_->AddComponent<LineSettingsComponent>(
    // entity,
    // (tmp++ == 0)                                              // Debug
    // ? LineSettingsComponent{false, false, 1, Qt::yellow}  // Debug
    // : LineSettingsComponent{}                             // Debug
    // );
    // scene_->AddComponent<PointSettingsComponent>(entity);
  }

  //  uint32_t count = scene_->GetEntities<Model>().size();
  //  emit signal_handler_.SetObjectsCount(QString::number(count));
}

void Backend::Update() {
  mousePickingSystem_->Update();
  cameraSystem_->Update();
  projectionSystem_->Update();
  lightSystem_->Update();

  // cSettings_.MoveCamera(CameraDirection::NONE);

  // if (cSettings_.MousePicking()) {
  //   cSettings_.MousePicking(false);
  //   const QPoint &mousePosition = cSettings_.GetMousePosition();
  //   mousePickingSystem_->Update(camera_, mousePosition)
  //       ? emit signal_handler_.SetStackObject()
  //       : emit signal_handler_.SetStackCamera();
  //   objectEditorSystem_->ObjectSelected();
  // }
}

void Backend::Draw() {
  {
    renderSystem_->Update();
    renderPickedSystem_->Update();
  }
  render2DSystem_->Update();
}

void Backend::Clear() {
  auto const& c = QColor{0, 0, 0};
  glClearColor(c.redF(), c.greenF(), c.blueF(), c.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Backend::RegisterComponents() {
  scene_.RegisterComponent<Quad>();
  scene_.RegisterComponent<Model>();
  scene_.RegisterComponent<Light>();
  scene_.RegisterComponent<Camera>();
  scene_.RegisterComponent<Cubemap>();
  scene_.RegisterComponent<Texture>();
  scene_.RegisterComponent<Material>();
  scene_.RegisterComponent<Transform>();
  scene_.RegisterComponent<SpotLight>();
  scene_.RegisterComponent<Projection>();
  scene_.RegisterComponent<PointLight>();
  scene_.RegisterComponent<PickingTag>();
  scene_.RegisterComponent<Attenuation>();
  scene_.RegisterComponent<DirectionalLight>();
}

void Backend::RegisterSystems() {
  cameraSystem_ = scene_.RegisterSystem<CameraSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Camera>());
    scene_.ChangeSystemMask<CameraSystem>(mask);
    cameraSystem_->Init(&scene_);
  }

  projectionSystem_ = scene_.RegisterSystem<ProjectionSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Projection>());
    scene_.ChangeSystemMask<ProjectionSystem>(mask);
    projectionSystem_->Init(&scene_);
  }

  mousePickingSystem_ = scene_.RegisterSystem<MousePickingSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Model>());
    mask.set(GetComponentID<Transform>());
    scene_.ChangeSystemMask<MousePickingSystem>(mask);
    mousePickingSystem_->Init(&scene_, technique_.get());
  }

  renderSystem_ = scene_.RegisterSystem<RenderSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Model>());
    mask.set(GetComponentID<Transform>());
    scene_.ChangeSystemMask<RenderSystem>(mask);
    renderSystem_->Init(&scene_, technique_.get());
  }

  render2DSystem_ = scene_.RegisterSystem<Render2DSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Quad>());
    mask.set(GetComponentID<Texture>());
    scene_.ChangeSystemMask<Render2DSystem>(mask);
    render2DSystem_->Init(&scene_, technique_.get());
  }

  renderPickedSystem_ = scene_.RegisterSystem<RenderPickedSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Model>());
    mask.set(GetComponentID<Transform>());
    mask.set(GetComponentID<PickingTag>());
    scene_.ChangeSystemMask<RenderPickedSystem>(mask);
    renderPickedSystem_->Init(&scene_, technique_.get());
  }

  lightSystem_ = scene_.RegisterSystem<LightSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Light>());
    scene_.ChangeSystemMask<LightSystem>(mask);
    lightSystem_->Init(&scene_, technique_.get());
  }
}

}  // namespace s21
