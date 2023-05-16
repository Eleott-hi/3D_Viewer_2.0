#include "Backend.h"

#include "Utils.h"

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

std::string
    dir =  //
           //            "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/";
    "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Tutorials/resources/";

namespace s21 {

void Backend::Init(QOpenGLWidget* widget) {
  initializeOpenGLFunctions();
  glLineStipple(4, 0xAAAA);

  opengl_widget_ = widget;
  technique_ = std::make_shared<TechniqueStrategy>();
  texture_storage_ = std::make_shared<TextureStorage>();
  parser_ = std::make_shared<Parser>(texture_storage_.get());

  g_buffer_ = std::make_shared<Framebuffer>();
  framebuffer3D_ = std::make_shared<Framebuffer>();
  framebufferShadow_ = std::make_shared<Framebuffer>();

  framebufferShadow_->Create({Format::DEPTH32});
  framebuffer3D_->Create({Format::RGB, Format::DEFAULT_DEPTH});
  g_buffer_->Create(
      {Format::RGBA16F, Format::RGBA16F, Format::RGBA, Format::DEFAULT_DEPTH});

  RegisterComponents();
  RegisterSystems();

  {
    Texture texture = {framebuffer3D_->getTextureID(), "quad"};
    //    Texture texture = {framebufferShadow_->getDepthID(), "quad"};
    //    Texture texture = {g_buffer_->getTextureID(2), "quad"};

    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<QuadTag>(entity);
    scene_.AddComponent<Texture>(entity, texture);
  }

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

    std::vector<std::string> faces{
        dir + "textures/skybox/right.jpg", dir + "textures/skybox/left.jpg",
        dir + "textures/skybox/top.jpg",   dir + "textures/skybox/bottom.jpg",
        dir + "textures/skybox/front.jpg", dir + "textures/skybox/back.jpg"};

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

  DebugLights(false, true, false, false);
}

void Backend::AddModel(QString path) {
  opengl_widget_->makeCurrent();
  auto [model, diffuse_map, normal_map, specular_map] =
      parser_->loadModel(path.toStdString());

  if (!model) return;

  for (auto& mesh : model->meshes) mesh.bufferize(this);

  Material material;
  material.shininess = 10;
  if (diffuse_map) material.diffuse = *diffuse_map;
  if (normal_map) material.normal = *normal_map;
  if (specular_map) material.specular = *specular_map;

  material.roughness =
      texture_storage_->LoadTexture(dir + "objects/backpack/roughness.jpg");

  material.ao = texture_storage_->LoadTexture(dir + "objects/backpack/ao.jpg");

  EntityID entity = scene_.NewEntity();
  scene_.AddComponent<Shader>(entity, {TechniqueType::LIGHT_TEXTURE});
  scene_.AddComponent<Transform>(entity);
  scene_.AddComponent<RenderTag>(entity);
  scene_.AddComponent<ShadowTag>(entity);
  scene_.AddComponent<Material>(entity, material);
  scene_.AddComponent<Model>(entity, std::move(*model));

  opengl_widget_->doneCurrent();
}

void Backend::LoadTexture(QString filename, Texture& texture) {
  InsideOpenGLContext(
      [&] { texture = texture_storage_->LoadTexture(filename.toStdString()); });
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

  if (picked_) {
    Notify();
    picked_ = false;
  }

  // NotifyCamera();
}

void Backend::Draw() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  {
    g_buffer_->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    defferedShadingSystem_->Update();

    g_buffer_->Unbind();
  }
  {  //

    framebufferShadow_->Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    shadowSystem_->Update();

    framebufferShadow_->Unbind();
  }

  {
    framebuffer3D_->Bind();
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    cubemapSystem_->Update();
    renderSystem_->Update();
    renderPickedSystem_->Update();

    framebuffer3D_->Unbind();
  }

  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.9, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glViewport(0, 0, width_, height_);
    render2DSystem_->Update(g_buffer_->getTextureID(0),
                            g_buffer_->getTextureID(1),
                            g_buffer_->getTextureID(2));
  }
}

void Backend::RegisterComponents() {
  scene_.RegisterComponent<Mesh>();
  scene_.RegisterComponent<Model>();
  scene_.RegisterComponent<Light>();
  scene_.RegisterComponent<Shader>();
  scene_.RegisterComponent<Camera>();
  scene_.RegisterComponent<Texture>();
  scene_.RegisterComponent<QuadTag>();
  scene_.RegisterComponent<Material>();
  scene_.RegisterComponent<InputTag>();
  scene_.RegisterComponent<RenderTag>();
  scene_.RegisterComponent<Transform>();
  scene_.RegisterComponent<ShadowTag>();
  scene_.RegisterComponent<CubemapTag>();
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
    mask.set(GetComponentID<Material>());
    mask.set(GetComponentID<RenderTag>());
    scene_.ChangeSystemMask<RenderSystem>(mask);
    renderSystem_->Init(&scene_, technique_.get());
  }

  shadowSystem_ = scene_.RegisterSystem<ShadowSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Model>());
    mask.set(GetComponentID<Transform>());
    mask.set(GetComponentID<ShadowTag>());
    mask.set(GetComponentID<RenderTag>());
    scene_.ChangeSystemMask<ShadowSystem>(mask);
    shadowSystem_->Init(&scene_, technique_.get());
  }

  defferedShadingSystem_ = scene_.RegisterSystem<DefferedShadingSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Model>());
    mask.set(GetComponentID<Transform>());
    scene_.ChangeSystemMask<DefferedShadingSystem>(mask);
    defferedShadingSystem_->Init(&scene_, technique_.get());
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

void Backend::DebugLights(bool directional, bool point_1, bool point_2,
                          bool spot) {
  if (directional) {
    Light light;
    light.type = LightType::DIRECTIONAL;
    // light.ambient = {150, 150, 150};
    light.ambient = {0.5, 0.5, 0.5};
    light.direction = {0, -1, 0};

    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Light>(entity, light);
    scene_.AddComponent<PickingTag>(entity);
  }

  if (point_1) {
    Attenuation attenuation;
    attenuation.constant = 0.1;
    attenuation.linear = 0.1;
    attenuation.quadratic = 0.1;

    Light light;
    light.type = LightType::POINT;
    light.position = {0, 0, 0};
    // light.ambient = {150, 150, 150};
    light.ambient = {0.5, 0.5, 0.5};
    light.specular = {0.5, 0.5, 0.5};

    Transform transform;
    transform.translation = {0, 0, 2};
    transform.scale = {0.1, 0.1, 0.1};

    Material material;
    material.color = Qt::white;

    auto model = *parser_->loadModel(dir + "objects/cube.obj").model;
    for (auto& mesh : model.meshes) mesh.bufferize(this);

    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Light>(entity, light);
    scene_.AddComponent<Material>(entity, material);
    scene_.AddComponent<Transform>(entity, transform);
    scene_.AddComponent<Model>(entity, std::move(model));
    scene_.AddComponent<Attenuation>(entity, attenuation);
    scene_.AddComponent<Shader>(entity, {TechniqueType::SIMPLE_COLOR});
    scene_.AddComponent<RenderTag>(entity);
  }

  if (point_2) {
    Attenuation attenuation;
    attenuation.constant = 0.1;
    attenuation.linear = 0.1;
    attenuation.quadratic = 0.1;

    Light light;
    light.type = LightType::POINT;
    light.position = {0, 0, 0};
    light.ambient = {50, 50, 50};
    // light.ambient = {0.5, 0.5, 0.5};
    light.specular = {0.5, 0.5, 0.5};

    Transform transform;
    transform.translation = {0, 0, 2};
    transform.scale = {0.1, 0.1, 0.1};

    Material material;
    material.color = Qt::white;

    auto model = *parser_->loadModel(dir + "objects/cube.obj").model;
    for (auto& mesh : model.meshes) mesh.bufferize(this);

    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Light>(entity, light);
    scene_.AddComponent<Material>(entity, material);
    scene_.AddComponent<Transform>(entity, transform);
    scene_.AddComponent<Model>(entity, std::move(model));
    scene_.AddComponent<Shader>(entity, {TechniqueType::SIMPLE_COLOR});
    scene_.AddComponent<Attenuation>(entity, attenuation);
    scene_.AddComponent<RenderTag>(entity);
  }

  if (spot) {
    Attenuation attenuation;
    attenuation.constant = 0.1;
    attenuation.linear = 0.1;
    attenuation.quadratic = 0.1;

    Light light;
    light.type = LightType::SPOT;
    // light.position = {0, 0, 0};
    // light.ambient = {50, 50, 50};
    light.ambient = {0.5, 0.5, 0.5};
    light.specular = {0.5, 0.5, 0.5};

    Transform transform;
    transform.translation = {0, 0, 2};
    transform.scale = {0.1, 0.1, 0.1};

    Material material;
    material.color = Qt::white;

    auto model = *parser_->loadModel(dir + "objects/cube.obj").model;
    for (auto& mesh : model.meshes) mesh.bufferize(this);

    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Light>(entity, light);
    scene_.AddComponent<Material>(entity, material);
    scene_.AddComponent<Transform>(entity, transform);
    scene_.AddComponent<Model>(entity, std::move(model));
    scene_.AddComponent<Shader>(entity, {TechniqueType::SIMPLE_COLOR});
    scene_.AddComponent<RenderTag>(entity);
    // scene_.AddComponent<Attenuation>(entity, attenuation);
  }
}

Camera& Backend::GetCamera() {
  qDebug() << "GetCamera";
  static auto& camera =
      scene_.GetComponent<Camera>(Utils::GetCameraID(&scene_));
  return camera;
}

void Backend::InsideOpenGLContext(std::function<void()> func) {
  opengl_widget_->makeCurrent();
  func();
  opengl_widget_->doneCurrent();
}

}  // namespace s21
