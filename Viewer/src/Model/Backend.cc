#include "Backend.h"

#include "events/KeyPressedEvent.h"
#include "events/KeyReleasedEvent.h"
#include "events/MouseDoubleClickedEvent.h"
#include "events/MouseMovedEvent.h"
#include "events/MouseScrolledEvent.h"
#include "events/WindowResizeEvent.h"

namespace s21 {

void Backend::Init(QOpenGLWidget* widget) {
  initializeOpenGLFunctions();

  opengl_widget_ = widget;
  technique_ = std::make_shared<TechniqueStrategy>();
  texture_storage_ = std::make_shared<TextureStorage>();
  parser_ = std::make_shared<Parser>(texture_storage_.get());

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

void Backend::AddModel(QString path) {
  opengl_widget_->makeCurrent();
  auto [model, diffuse_map, normal_map] = parser_->loadModel(path.toStdString());

  if (!model) return;

  for (auto& mesh : model->meshes)
    if (!mesh.VAO) mesh.bufferize(this);

  Material material;

  if (diffuse_map) material.diffuse = diffuse_map->id;
  if (normal_map) material.normal = normal_map->id;

  EntityID entity = scene_.NewEntity();
  scene_.AddComponent<Model>(entity, std::move(model.value()));
  scene_.AddComponent<Material>(entity, material);
  scene_.AddComponent<Transform>(entity);

  opengl_widget_->doneCurrent();
}

void Backend::LoadTexture(QString filename) {
  opengl_widget_->makeCurrent();
  auto id = texture_storage_->loadTexture(filename.toStdString());
  opengl_widget_->doneCurrent();
}

void Backend::Render() {
  Update();
  Draw();
}

void Backend::Update() {
  mousePickingSystem_->Update();
  cameraSystem_->Update();
  projectionSystem_->Update();
  lightSystem_->Update();
}

void Backend::Draw() {
  {
    renderSystem_->Update();
    cubemapSystem_->Update();
    renderPickedSystem_->Update();
  }

  render2DSystem_->Update();
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

  cubemapSystem_ = scene_.RegisterSystem<CubemapSystem>();
  {
    ComponentMask mask;
    cubemapSystem_->Init(&scene_, technique_.get());
  }
}

}  // namespace s21
