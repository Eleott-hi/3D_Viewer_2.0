#include "Backend.h"

namespace s21 {

std::string cube =
    "/opt/goinfre/pintoved/3D_Viewer_2.0/Viewer/src/others/resources/cube.obj";

void Backend::Init(QOpenGLWidget* widget) {
  initializeOpenGLFunctions();

  opengl_widget_ = widget;
  technique_ = std::make_shared<TechniqueStrategy>();
  texture_storage_ = std::make_shared<TextureStorage>();
  parser_ = std::make_shared<Parser>(texture_storage_.get());

  glLineStipple(4, 0xAAAA);

  RegisterComponents();
  RegisterSystems();

  {
    Camera camera;
    camera.position = {0, 0, 6};
    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Camera>(entity, camera);
  }

  {
    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Projection>(entity);
  }

  {
    Attenuation attenuation;
    attenuation.constant = 0.1;
    attenuation.linear = 0.1;
    attenuation.quadratic = 0.1;

    Light light;
    light.type = LightType::POINT;
    light.position = {0, 0, 0};
    light.specular = {0.5, 0.5, 0.5};

    Transform transform;
    transform.translation = {0, 0, 2};

    // auto model = *parser_->loadModel(cube).model;
    // for (auto& mesh : model.meshes) mesh.bufferize(this);

    EntityID entity = scene_.NewEntity();
    // scene_.AddComponent<Material>(entity);
    scene_.AddComponent<Light>(entity, light);
    // scene_.AddComponent<Transform>(entity, transform);
    // scene_.AddComponent<Model>(entity, std::move(model));
    scene_.AddComponent<Attenuation>(entity, attenuation);
  }
}

void Backend::AddModel(QString path) {
  opengl_widget_->makeCurrent();
  auto [model, diffuse_map, normal_map, specular_map] =
      parser_->loadModel(path.toStdString());

  if (!model) return;

  QVector<EntityID> meshes;

  for (auto& mesh : model->meshes) {
    mesh.bufferize(this);

    Material material;
    material.shininess = 10;
    if (diffuse_map) material.diffuse = diffuse_map->id;
    if (normal_map) material.normal = normal_map->id;
    if (specular_map) material.specular = specular_map->id;

    material.roughness = texture_storage_->loadTexture(
        "/opt/goinfre/pintoved/3D_Viewer_2.0/Viewer/src/others/resources/"
        "backpack/roughness.jpg");
    material.ao = texture_storage_->loadTexture(
        "/opt/goinfre/pintoved/3D_Viewer_2.0/Viewer/src/others/resources/"
        "backpack/ao.jpg");

    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Shader>(entity, {TechniqueType::LIGHT_TEXTURE});
    scene_.AddComponent<Transform>(entity);
    scene_.AddComponent<Material>(entity, material);
    scene_.AddComponent<Model>(entity, std::move(*model));

    meshes << entity;
  }

  NewModel new_model;
  new_model.filename = path.toStdString();
  new_model.meshes = meshes;

  EntityID entity = scene_.NewEntity();
  scene_.AddComponent<Shader>(entity, {TechniqueType::LIGHT_TEXTURE});
  scene_.AddComponent<Transform>(entity);
  scene_.AddComponent<NewModel>(entity, std::move(*new_model));

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
  editPickedSystem_->Update();
  cameraSystem_->Update();
  projectionSystem_->Update();
  lightSystem_->Update();
}

void Backend::Draw() {
  {
    renderSystem_->Update();
    // cubemapSystem_->Update();
    renderPickedSystem_->Update();
  }

  render2DSystem_->Update();
}

void Backend::RegisterComponents() {
  scene_.RegisterComponent<Quad>();
  scene_.RegisterComponent<NewModel>();
  scene_.RegisterComponent<Mesh>();
  scene_.RegisterComponent<Model>();
  scene_.RegisterComponent<Light>();
  scene_.RegisterComponent<Shader>();
  scene_.RegisterComponent<Camera>();
  scene_.RegisterComponent<Cubemap>();
  scene_.RegisterComponent<Texture>();
  scene_.RegisterComponent<Material>();
  scene_.RegisterComponent<Transform>();
  //  scene_.RegisterComponent<SpotLight>();
  scene_.RegisterComponent<Projection>();
  //  scene_.RegisterComponent<PointLight>();
  scene_.RegisterComponent<PickingTag>();
  scene_.RegisterComponent<Attenuation>();
  //  scene_.RegisterComponent<DirectionalLight>();
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
    mask.set(GetComponentID<NewModel>());
    mask.set(GetComponentID<Transform>());
    scene_.ChangeSystemMask<MousePickingSystem>(mask);
    mousePickingSystem_->Init(&scene_, technique_.get());
  }

  renderSystem_ = scene_.RegisterSystem<RenderSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<NewModel>());
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
    mask.set(GetComponentID<NewModel>());
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

  editPickedSystem_ = scene_.RegisterSystem<EditPickedSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<PickingTag>());
    scene_.ChangeSystemMask<EditPickedSystem>(mask);
    editPickedSystem_->Init(&scene_);
  }
}

}  // namespace s21
