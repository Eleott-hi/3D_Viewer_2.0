#include "RenderPickedSystem.h"

namespace s21 {

void RenderPickedSystem::Init(
    std::shared_ptr<ECS_Controller> const &scene,
    std::shared_ptr<TechniqueStrategy> const &technique) {
  scene_ = scene;
  technique_ = technique;
}

void RenderPickedSystem::Update(EntityID camera) {
  auto &[proj, view] = scene_->GetComponent<CameraComponent>(camera);

  technique_->Enable(TechniqueType::SIMPLE_COLOR);

  for (auto &entity : entities_) {
    auto &transform = scene_->GetComponent<TransformComponent>(entity);
    auto &model = scene_->GetComponent<MeshComponent>(entity);

    QMatrix4x4 modelMatrix = transform.GetModelMatrix();

    // ========================== STENCIL BUFFER ==========================
    glEnable(GL_STENCIL_TEST);  // Enable stancil buffer
    glDisable(GL_DEPTH_TEST);   // Disable depth buffer

    glStencilFunc(GL_ALWAYS, 1, 0xFF);          // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // Set rules
    glStencilMask(0xFF);                        // Write to stancil buffer
    glClear(GL_STENCIL_BUFFER_BIT);             // Clear stencil buffer

    // Disable writing to color buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // Draw model to stencil buffer
    technique_->setMVP(proj, view, modelMatrix);
    DrawObject(model);

    // Enable writing to color buffer
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // pixel of stencil buffer != 1
    glStencilMask(0x00);                  // Disable writing to stencil buffer

    // Draw outline to stencil buffer
    modelMatrix.scale(1.02);  // TODO (pintoved): check with other scales
    technique_->setColor(Qt::white);
    technique_->setMVP(proj, view, modelMatrix);
    DrawObject(model);

    glDisable(GL_STENCIL_TEST);  // Disable stancil buffer
    glEnable(GL_DEPTH_TEST);     // Enable depth buffer
    // ========================== STENCIL BUFFER ==========================
  }
}

void RenderPickedSystem::DrawObject(MeshComponent &model) {
  for (auto &mesh : model.meshes_) {
    if (!mesh.VAO) Bufferize(mesh);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

void RenderPickedSystem::Bufferize(s_Mesh &mesh) {
  // auto &[VAO, vertices, indices] = mesh;
  // uint32_t VBO = 0, EBO = 0;
  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  // glBindVertexArray(VAO);

  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
  //              vertices.data(), GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(),
  //              indices.data(), GL_STATIC_DRAW);

  // // vertex Positions
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (void *)offsetof(Vertex, Position));
  // // vertex normals
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (void *)offsetof(Vertex, Normal));
  // // vertex texture coords
  // glEnableVertexAttribArray(2);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       (void *)offsetof(Vertex, TexCoords));
  // glBindVertexArray(0);
}

}  // namespace s21
