#include "RenderPickedSystem.h"

#include "Utils.h"

namespace s21 {

void RenderPickedSystem::Init(ECS_Controller *scene,
                              TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void RenderPickedSystem::Update() {
  if (entities_.empty()) return;

  std::unordered_map<int, QMatrix4x4> matricies;
  std::function<QMatrix4x4(EntityID)> get_model_matrix = [&](EntityID entity) {
    if (matricies.find(entity) == matricies.end() &&
        scene_->EntityHasComponent<Transform>(entity)) {
      auto &transform = scene_->GetComponent<Transform>(entity);
      auto &hierarchy = scene_->GetComponent<HierarchyComponent>(entity);

      matricies[entity] =
          get_model_matrix(hierarchy.parent) * transform.GetModelMatrix();
    }

    return matricies[entity];
  };

  // ========================== STENCIL BUFFER ==========================
  glEnable(GL_STENCIL_TEST);  // Enable stancil buffer
  glDisable(GL_DEPTH_TEST);   // Disable depth buffer

  for (auto &entity : entities_) {
    // auto const &transform = scene_->GetComponent<Transform>(entity);
    auto &model = scene_->GetComponent<Mesh>(entity);

    auto const &modelMatrix = get_model_matrix(entity);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);          // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // Set rules
    glStencilMask(0xFF);                        // Write to stancil buffer
    glClear(GL_STENCIL_BUFFER_BIT);             // Clear stencil buffer

    // Disable writing to color buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // Draw model to stencil buffer
    technique_->Enable(TechniqueType::SIMPLE_COLOR);
    technique_->SetModelMatrix(modelMatrix);
    model.Draw(this, GL_TRIANGLES);

    // Enable writing to color buffer
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // pixel of stencil buffer != 1
    glStencilMask(0x00);                  // Disable writing to stencil buffer

    // Draw outline to stencil buffer
    technique_->Enable(TechniqueType::STENCIL_OUTLINE);
    technique_->SetModelMatrix(modelMatrix);
    model.Draw(this, GL_TRIANGLES);
  }

  glDisable(GL_STENCIL_TEST);  // Disable stancil buffer
  glEnable(GL_DEPTH_TEST);     // Enable depth buffer
  // ========================== STENCIL BUFFER ==========================
}

}  // namespace s21
