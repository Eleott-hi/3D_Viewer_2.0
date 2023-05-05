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

void Backend::Render() {
  Update();
  Draw();
}

void Backend::AddModel(QString path) {
  opengl_widget_->makeCurrent();
  auto [model, normalMap] = parser_.loadModel(path);

  if (model) {
    for (auto& mesh : model->meshes)
      if (!mesh.VAO) {
        qDebug() << "HERE";
        mesh.bufferize(this);
      }

    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Model>(entity, std::move(model.value()));
    scene_.AddComponent<Material>(entity);
    scene_.AddComponent<Transform>(entity);
  }

  // if (normalMap) {
  //   EntityID entity = scene_.NewEntity();
  //  }

  opengl_widget_->doneCurrent();
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
    renderPickedSystem_->Update();
    cubemapSystem_->Update();
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

  cubemapSystem_ = scene_.RegisterSystem<CubemapSystem>();
  {
    ComponentMask mask;
    cubemapSystem_->Init(&scene_, technique_.get());
  }
}

}  // namespace s21
