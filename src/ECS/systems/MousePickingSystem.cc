#include "MousePickingSystem.h"

namespace s21 {

MousePickingSystem::MousePickingSystem() {
  initializeOpenGLFunctions();
  pickingFramebuffer_ = std::make_unique<Framebuffer>();
  pickingFramebuffer_->Create({Format::RED_INTEGER, Format::DEFAULT_DEPTH});
}

void MousePickingSystem::Init(
    std::shared_ptr<Controller> const &scene,
    std::shared_ptr<TechniqueStrategy> const &technique) {
  scene_ = scene;
  technique_ = technique;
  scene_->AddEventListener(EventType::WindowResize,
                           BIND_EVENT_FN(OnWindowResize));
}

void MousePickingSystem::OnWindowResize(Event &event) {
  auto &cast_event = static_cast<WindowResizeEvent &>(event);
  pickingFramebuffer_->Resize(cast_event.GetWidth(), cast_event.GetHeight());
}

bool MousePickingSystem::Update(EntityID camera, const QPoint &pos) {
  auto &[proj, view] = scene_->GetComponent<CameraComponent>(camera);

  pickingFramebuffer_->Bind();
  glClearColor(-1, -1, -1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for (auto &entity : entities_) {
    auto const &transform = scene_->GetComponent<TransformComponent>(entity);
    auto &model = scene_->GetComponent<MeshComponent>(entity);

    QMatrix4x4 modelMatrix = transform.GetModelMatrix();

    technique_->Enable(TechniqueType::MOUSE_PICKING);
    technique_->setMVP(proj, view, modelMatrix);
    technique_->SetObjectID((int)entity);

    DrawObject(model);
  }
  pickingFramebuffer_->Unbind();

  int objectID = pickingFramebuffer_->ReadPixel(pos.x(), pos.y());

  auto pickedEntities = scene_->GetEntities<PickingComponent>();

  for (auto &entity : pickedEntities)
    scene_->RemoveComponent<PickingComponent>(entity);

  if (objectID >= 0) scene_->AddComponent<PickingComponent>(objectID);

  qDebug() << objectID;

  return objectID >= 0;
}

void MousePickingSystem::DrawObject(MeshComponent &model) {
  for (auto &mesh : model.meshes_) {
    if (!mesh.VAO) bufferize(mesh);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

void MousePickingSystem::bufferize(s_Mesh &mesh) {
  auto &[VAO, vertices, indices] = mesh;
  uint32_t VBO = 0, EBO = 0;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  // vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Position));
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));
  glBindVertexArray(0);
}

}  // namespace s21
