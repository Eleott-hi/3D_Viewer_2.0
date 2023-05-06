#include "RenderPickedSystem.h"

#include "Utils.h"

namespace s21 {

void RenderPickedSystem::Init(ECS_Controller *scene,
                              TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void RenderPickedSystem::Update() {
  auto [proj, view] = Utils::GetProjectionAndView(scene_);

  for (auto &entity : entities_) {
    auto const &transform = scene_->GetComponent<Transform>(entity);
    auto const &model = scene_->GetComponent<Model>(entity);

    auto const &modelMatrix = transform.GetModelMatrix();

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
    technique_->Enable(TechniqueType::SIMPLE_COLOR);
    technique_->setMVP(proj, view, modelMatrix);
    DrawObject(model);

    // Enable writing to color buffer
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // pixel of stencil buffer != 1
    glStencilMask(0x00);                  // Disable writing to stencil buffer

    // Draw outline to stencil buffer
    technique_->Enable(TechniqueType::STENCIL_OUTLINE);
    technique_->setMVP(proj, view, modelMatrix);
    DrawObject(model);

    glDisable(GL_STENCIL_TEST);  // Disable stancil buffer
    glEnable(GL_DEPTH_TEST);     // Enable depth buffer
    // ========================== STENCIL BUFFER ==========================
  }
}

void RenderPickedSystem::DrawObject(Model const &model) {
  for (auto const &mesh : model.meshes) {
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

}  // namespace s21
