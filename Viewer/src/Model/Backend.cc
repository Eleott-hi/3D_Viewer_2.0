#include "Backend.h"

float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,
    -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,
    -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,
};

namespace s21 {

std::string cube =
    "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/cube.obj";

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
    uint32_t skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);

    std::string dir =
        "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/textures/"
        "skybox"
        // "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Tutorials/resources/"
        // "textures/skybox"  //
        ;

    std::vector<std::string> faces{dir + "/right.jpg", dir + "/left.jpg",
                                   dir + "/top.jpg",   dir + "/bottom.jpg",
                                   dir + "/front.jpg", dir + "/back.jpg"};

    Mesh mesh;
    mesh.VAO = skyboxVAO;

    Texture texture;
    texture.type = "cubemap";
    texture.id = texture_storage_->LoadCubemap(faces);

    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Mesh>(entity, mesh);
    scene_.AddComponent<CubemapTag>(entity);
    scene_.AddComponent<Texture>(entity, texture);
  }

  {
    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<InputTag>(entity);
    scene_.AddComponent<KeyboardInput>(entity);
  }

  {
    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<InputTag>(entity);
    scene_.AddComponent<MouseInput>(entity);
  }

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

  // {
  //   Light light;
  //   light.type = LightType::DIRECTIONAL;
  //   light.ambient = {150, 150, 150};
  //   light.direction = {0, -1, 0};

  //   EntityID entity = scene_.NewEntity();
  //   scene_.AddComponent<Light>(entity, light);
  // }

  // {
  //   Attenuation attenuation;
  //   attenuation.constant = 0.1;
  //   attenuation.linear = 0.1;
  //   attenuation.quadratic = 0.1;

  //   Light light;
  //   light.type = LightType::POINT;
  //   light.position = {0, 0, 0};
  //   light.ambient = {150, 150, 150};
  //   // light.ambient = {0.5, 0.5, 0.5};
  //   light.specular = {0.5, 0.5, 0.5};

  //   Transform transform;
  //   transform.translation = {0, 0, 2};
  //   transform.scale = {0.1, 0.1, 0.1};

  //   Material material;
  //   material.color = Qt::white;

  //   auto model = *parser_->loadModel(cube).model;
  //   for (auto& mesh : model.meshes) mesh.bufferize(this);

  //   EntityID entity = scene_.NewEntity();
  //   scene_.AddComponent<Light>(entity, light);
  //   scene_.AddComponent<Material>(entity, material);
  //   scene_.AddComponent<Transform>(entity, transform);
  //   scene_.AddComponent<Model>(entity, std::move(model));
  //   scene_.AddComponent<Attenuation>(entity, attenuation);
  // }

  // {
  //   Attenuation attenuation;
  //   attenuation.constant = 0.1;
  //   attenuation.linear = 0.1;
  //   attenuation.quadratic = 0.1;

  //   Light light;
  //   light.type = LightType::POINT;
  //   light.position = {0, 0, 0};
  //   light.ambient = {50, 50, 50};
  //   // light.ambient = {0.5, 0.5, 0.5};
  //   light.specular = {0.5, 0.5, 0.5};

  //   Transform transform;
  //   transform.translation = {0, 0, 2};
  //   transform.scale = {0.1, 0.1, 0.1};

  //   Material material;
  //   material.color = Qt::white;

  //   auto model = *parser_->loadModel(cube).model;
  //   for (auto& mesh : model.meshes) mesh.bufferize(this);

  //   EntityID entity = scene_.NewEntity();
  //   scene_.AddComponent<Light>(entity, light);
  //   scene_.AddComponent<Material>(entity, material);
  //   scene_.AddComponent<Transform>(entity, transform);
  //   scene_.AddComponent<Model>(entity, std::move(model));
  //   scene_.AddComponent<Attenuation>(entity, attenuation);
  // }

  // {
  //   Attenuation attenuation;
  //   attenuation.constant = 0.1;
  //   attenuation.linear = 0.1;
  //   attenuation.quadratic = 0.1;

  //   Light light;
  //   light.type = LightType::SPOT;
  //   // light.position = {0, 0, 0};
  //   light.ambient = {50, 50, 50};
  //   // light.ambient = {0.5, 0.5, 0.5};
  //   // light.specular = {0.5, 0.5, 0.5};

  //   Transform transform;
  //   transform.translation = {0, 0, 2};
  //   transform.scale = {0.1, 0.1, 0.1};

  //   Material material;
  //   material.color = Qt::white;

  //   auto model = *parser_->loadModel(cube).model;
  //   for (auto& mesh : model.meshes) mesh.bufferize(this);

  //   EntityID entity = scene_.NewEntity();
  //   scene_.AddComponent<Light>(entity, light);
  //   scene_.AddComponent<Material>(entity, material);
  //   scene_.AddComponent<Transform>(entity, transform);
  //   scene_.AddComponent<Model>(entity, std::move(model));
  //   // scene_.AddComponent<Attenuation>(entity, attenuation);
  // }
}

void Backend::AddModel(QString path) {
  opengl_widget_->makeCurrent();
  auto [model, diffuse_map, normal_map, specular_map] =
      parser_->loadModel(path.toStdString());

  if (!model) return;

  for (auto& mesh : model->meshes) mesh.bufferize(this);

  Material material;
  material.shininess = 10;
  if (diffuse_map) material.diffuse = diffuse_map->id;
  if (normal_map) material.normal = normal_map->id;
  if (specular_map) material.specular = specular_map->id;

  material.roughness = texture_storage_->LoadTexture(
      "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/"
      "backpack/roughness.jpg"
      // "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Tutorials/resources/"
      // "backpack/roughness.jpg"  //
  );

  material.ao = texture_storage_->LoadTexture(
      "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/"
      "backpack/ao.jpg"
      // "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Tutorials/resources/"
      // "backpack/ao.jpg"  //
  );

  EntityID entity = scene_.NewEntity();
  scene_.AddComponent<Shader>(entity, {TechniqueType::SIMPLE_TEXTURE});
  scene_.AddComponent<Transform>(entity);
  scene_.AddComponent<Material>(entity, material);
  scene_.AddComponent<Model>(entity, std::move(*model));

  opengl_widget_->doneCurrent();
}

void Backend::LoadTexture(QString filename) {
  opengl_widget_->makeCurrent();
  auto id = texture_storage_->LoadTexture(filename.toStdString());
  opengl_widget_->doneCurrent();
}

void Backend::Render() {
  Update();
  Draw();
}

void Backend::Update() {
  inputSystem_->Update();
  mousePickingSystem_->Update();
  editPickedSystem_->Update();
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
  scene_.RegisterComponent<QuadTag>();
  scene_.RegisterComponent<Mesh>();
  scene_.RegisterComponent<Model>();
  scene_.RegisterComponent<Light>();
  scene_.RegisterComponent<Shader>();
  scene_.RegisterComponent<Camera>();
  scene_.RegisterComponent<CubemapTag>();
  scene_.RegisterComponent<Texture>();
  scene_.RegisterComponent<Material>();
  scene_.RegisterComponent<InputTag>();
  scene_.RegisterComponent<Transform>();
  scene_.RegisterComponent<Enviroment>();
  scene_.RegisterComponent<MouseInput>();
  scene_.RegisterComponent<Projection>();
  scene_.RegisterComponent<PickingTag>();
  scene_.RegisterComponent<Attenuation>();
  scene_.RegisterComponent<KeyboardInput>();
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
    mask.set(GetComponentID<QuadTag>());
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
    mask.set(GetComponentID<Mesh>());
    mask.set(GetComponentID<Texture>());
    mask.set(GetComponentID<CubemapTag>());
    scene_.ChangeSystemMask<CubemapSystem>(mask);
    cubemapSystem_->Init(&scene_, technique_.get());
  }

  editPickedSystem_ = scene_.RegisterSystem<EditPickedSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<PickingTag>());
    scene_.ChangeSystemMask<EditPickedSystem>(mask);
    editPickedSystem_->Init(&scene_);
  }

  inputSystem_ = scene_.RegisterSystem<InputSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<InputTag>());
    scene_.ChangeSystemMask<InputSystem>(mask);
    inputSystem_->Init(&scene_);
  }
}

}  // namespace s21
