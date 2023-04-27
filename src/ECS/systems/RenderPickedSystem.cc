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
    auto &model = scene_->GetComponent<ModelComponent>(entity);

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

void RenderPickedSystem::DrawObject(ModelComponent &model) {
  for (auto &mesh : model.meshes_) {
    if (!mesh.VAO) mesh.bufferize(this);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

}  // namespace s21
