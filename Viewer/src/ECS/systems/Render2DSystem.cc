#include "Render2DSystem.h"

#include "Utils.h"

namespace s21 {

void Render2DSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void Render2DSystem::Update(uint32_t pos, uint32_t normal,
                            uint32_t albedo_spec) {
  static auto const &texture =
      scene_->GetComponent<Texture>(Utils::GetQuad(scene_));

  technique_->Enable(TechniqueType::QUAD);
  technique_->Clear();
  technique_->SetTexture({texture.id, "screenTexture"});
  technique_->SetTexture({pos, "gPosition"});
  technique_->SetTexture({normal, "gNormal"});
  technique_->SetTexture({albedo_spec, "gAlbedoSpec"});

  renderQuad();
}

void Render2DSystem::renderQuad() {
  static uint32_t quadVAO = 0;

  if (quadVAO == 0) {
    float quadVertices[] = {
        // positions       // texture Coords
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  //
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  //
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,  //
    };

    uint32_t quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
  }

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

}  // namespace s21
