#include "Scene.h"

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
           // "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/";
    "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Tutorials/resources/";

namespace s21 {

void Scene::Init(QOpenGLWidget* widget) {
  initializeOpenGLFunctions();
  glLineStipple(4, 0xAAAA);

  gizmo_ctx.render = [&](const tinygizmo::geometry_mesh& r) {
    // Render logic
  };

  // transform -> rigid_transform
  tinygizmo::rigid_transform xform_a;

  opengl_widget_ = widget;
  technique_ = std::make_shared<TechniqueStrategy>();
  texture_storage_ = std::make_shared<TextureStorage>();
  parser_ = std::make_shared<Parser>(texture_storage_.get());

  SetFramebuffers();
  RegisterComponents();
  RegisterSystems();
  InitEntities();
}

void Scene::Update() {
  timeTickSystem_->Update();
  // inputSystem_->Update();
  mousePickingSystem_->Update();
  editPickedSystem_->Update();
  cameraSystem_->Update();
  // projectionSystem_->Update();
  lightSystem_->Update();

  if (picked_) {
    Notify();
    picked_ = false;
  }

  NotifyCamera();
}

void Scene::Draw() {
  // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // {
  //   g_buffer_->Bind();
  //   g_buffer_->PrepereBuffer();
  //   defferedShadingSystem_->Update();
  //   g_buffer_->Unbind();
  // }

  // {
  //   framebufferShadow_->Bind();
  //   framebufferShadow_->PrepereBuffer();
  //   shadowSystem_->Update();
  //   framebufferShadow_->Unbind();
  // }

  // {
  //   framebuffer3D_->Bind();
  //   framebuffer3D_->PrepereBuffer();
  //   shadowRenderSystem_->Update(framebufferShadow_->getTextureID());
  //   framebuffer3D_->Unbind();
  // }

  {
    framebuffer3D_->Bind();
    framebuffer3D_->PrepereBuffer();

    cubemapSystem_->Update();
    renderSystem_->Update();
    renderPickedSystem_->Update();

    framebuffer3D_->Unbind();
  }

  // Draw Gizmo(){
  // gizmo_ctx.update(gizmo_state);

  // if (transform_gizmo("first-example-gizmo", gizmo_ctx, xform_a))
  // {
  // std::cout << get_local_time_ns() << " - " << "First Gizmo Hovered..." <<
  // std::endl; if (xform_a != xform_a_last) std::cout << get_local_time_ns() <<
  // " - " << "First Gizmo Changed..." << std::endl; xform_a_last = xform_a;
  // }

  // transform_gizmo("second-example-gizmo", gizmo_ctx, xform_b);
  // gizmo_ctx.draw();
  // }

  // {
  //   pointShadowFramebuffer_->Bind();
  //   pointShadowFramebuffer_->PrepereBuffer();
  //   pointShadowSystem_->Update();
  //   pointShadowFramebuffer_->Unbind();
  // }

  // {
  //   framebuffer3D_->Bind();
  //   framebuffer3D_->PrepereBuffer();
  //   cubemapSystem_->Update();
  //   pointShadowRenderSystem_->Update(pointShadowFramebuffer_->getTextureID());
  //   framebuffer3D_->Unbind();
  // }

  {
    defaultFramebuffer_->Bind();
    defaultFramebuffer_->PrepereBuffer();

    render2DSystem_->Update(g_buffer_->getTextureID(0),
                            g_buffer_->getTextureID(1),
                            g_buffer_->getTextureID(2));

    defaultFramebuffer_->Unbind();
  }
}

void Scene::Render() {
  Update();
  Draw();
}

void Scene::AddModel(QString path) {
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
  scene_.AddComponent<Shader>(entity,
                              {TechniqueType::PHYSICAL_BASED_RENDERING});
  scene_.AddComponent<Transform>(entity);
  scene_.AddComponent<RenderTag>(entity);
  scene_.AddComponent<ShadowTag>(entity);
  scene_.AddComponent<Material>(entity, material);
  scene_.AddComponent<Model>(entity, std::move(*model));

  opengl_widget_->doneCurrent();
}

void Scene::LoadTexture(QString filename, Texture& texture) {
  opengl_widget_->makeCurrent();
  texture = texture_storage_->LoadTexture(filename.toStdString());
  opengl_widget_->doneCurrent();
}

void Scene::RegisterComponents() {
  scene_.RegisterComponent<Mesh>();
  scene_.RegisterComponent<Input>();
  scene_.RegisterComponent<Model>();
  scene_.RegisterComponent<Light>();
  scene_.RegisterComponent<Timer>();
  scene_.RegisterComponent<Shader>();
  scene_.RegisterComponent<Camera>();
  scene_.RegisterComponent<Texture>();
  scene_.RegisterComponent<QuadTag>();
  scene_.RegisterComponent<Material>();
  scene_.RegisterComponent<RenderTag>();
  scene_.RegisterComponent<Transform>();
  scene_.RegisterComponent<ShadowTag>();
  scene_.RegisterComponent<CubemapTag>();
  scene_.RegisterComponent<Enviroment>();
  scene_.RegisterComponent<Projection>();
  scene_.RegisterComponent<PickingTag>();
  scene_.RegisterComponent<Attenuation>();
}

void Scene::RegisterSystems() {
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

  pointShadowSystem_ = scene_.RegisterSystem<PointShadowSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Model>());
    mask.set(GetComponentID<Transform>());
    mask.set(GetComponentID<ShadowTag>());
    mask.set(GetComponentID<RenderTag>());
    scene_.ChangeSystemMask<PointShadowSystem>(mask);
    pointShadowSystem_->Init(&scene_, technique_.get());
  }

  pointShadowRenderSystem_ = scene_.RegisterSystem<PointShadowRenderSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Model>());
    mask.set(GetComponentID<Transform>());
    mask.set(GetComponentID<Material>());
    // mask.set(GetComponentID<ShadowTag>());
    mask.set(GetComponentID<RenderTag>());
    scene_.ChangeSystemMask<PointShadowRenderSystem>(mask);
    pointShadowRenderSystem_->Init(&scene_, technique_.get());
  }

  shadowRenderSystem_ = scene_.RegisterSystem<ShadowRenderSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Model>());
    mask.set(GetComponentID<Transform>());
    mask.set(GetComponentID<Material>());
    // mask.set(GetComponentID<ShadowTag>());
    mask.set(GetComponentID<RenderTag>());
    scene_.ChangeSystemMask<ShadowRenderSystem>(mask);
    shadowRenderSystem_->Init(&scene_, technique_.get());
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
    mask.set(GetComponentID<Input>());
    scene_.ChangeSystemMask<InputSystem>(mask);
    inputSystem_->Init(&scene_);
  }

  timeTickSystem_ = scene_.RegisterSystem<TimeTickSystem>();
  {
    ComponentMask mask;
    mask.set(GetComponentID<Timer>());
    scene_.ChangeSystemMask<TimeTickSystem>(mask);
    timeTickSystem_->Init(&scene_);
  }
}

void Scene::DebugLights(bool directional, bool point_1, bool point_2,
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
    light.ambient = {150, 150, 150};
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

Camera& Scene::GetCamera() {
  qDebug() << "GetCamera";
  static auto& camera = scene_.GetComponent<Camera>(Utils::GetCamera(&scene_));
  return camera;
}

void Scene::InsideOpenGLContext(std::function<void()> func) {
  opengl_widget_->makeCurrent();
  func();
  opengl_widget_->doneCurrent();
}

void Scene::SetFramebuffers() {
  TextureWraper texture(GL_TEXTURE_CUBE_MAP);
  texture.SetFilters(GL_NEAREST);
  texture.SetWraps(GL_CLAMP_TO_EDGE);
  texture.SetAttachment(GL_DEPTH_ATTACHMENT);
  texture.SetFormats(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);

  g_buffer_ = std::make_shared<Framebuffer>();
  framebuffer3D_ = std::make_shared<Framebuffer>();
  framebufferShadow_ = std::make_shared<Framebuffer>();
  pointShadowFramebuffer_ = std::make_shared<Framebuffer>();
  // mousePickingFramebuffer_ = std::make_shared<Framebuffer>();
  defaultFramebuffer_ = std::make_shared<Framebuffer>();

  pointShadowFramebuffer_->AddTexture(texture);
  pointShadowFramebuffer_->Create({});
  framebufferShadow_->Create({Format::DEPTH32});
  framebuffer3D_->Create({Format::RGB, Format::DEFAULT_DEPTH});
  // mousePickingFramebuffer_->Create(
  // {Format::RED_INTEGER, Format::DEFAULT_DEPTH});
  g_buffer_->Create({Format::RGBA16F, Format::RGBA16F,  //
                     Format::RGBA, Format::DEFAULT_DEPTH});

  pointShadowFramebuffer_->Resize(1024, 1024);

  defaultFramebuffer_->SetPrepereBuffer([this] {
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.9, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  });

  pointShadowFramebuffer_->SetPrepereBuffer([this] {
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  });

  framebuffer3D_->SetPrepereBuffer([this] {
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  });

  framebufferShadow_->SetPrepereBuffer([this] {
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  });

  g_buffer_->SetPrepereBuffer([this] {
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.9f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  });
}

void Scene::InitEntities() {
  {
    Texture texture = {framebuffer3D_->getTextureID(), "quad"};
    // Texture texture = {framebufferShadow_->getTextureID(), "quad"};
    // Texture texture = {g_buffer_->getTextureID(1), "quad"};

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
    scene_.AddComponent<Input>(entity);
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

  {
    EntityID entity = scene_.NewEntity();
    scene_.AddComponent<Timer>(entity);
  }

  // {
  //   Transform transform;
  //   transform.translation = {0, -4, 0};
  //   transform.scale = {11, 1, 11};

  //   auto model = *parser_->loadModel(dir + "objects/cube.obj").model;
  //   for (auto& mesh : model.meshes) mesh.bufferize(this);

  //   EntityID entity = scene_.NewEntity();
  //   scene_.AddComponent<Material>(entity);
  //   scene_.AddComponent<Transform>(entity, transform);
  //   scene_.AddComponent<Model>(entity, std::move(model));
  //   scene_.AddComponent<Shader>(entity, {TechniqueType::SIMPLE_COLOR});
  //   scene_.AddComponent<RenderTag>(entity);
  //   scene_.AddComponent<ShadowTag>(entity);
  // }

  DebugLights(false, true, false, false);
}

}  // namespace s21
