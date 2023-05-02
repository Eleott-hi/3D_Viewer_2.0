#include "Render2DSystem.h"

#include "Utils.h"

namespace s21 {

void Render2DSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void Render2DSystem::Update() {
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  // glClear(GL_COLOR_BUFFER_BIT);
  // glDisable(GL_DEPTH_TEST);



  // technique_->Enable(TechniqueType::SIMPLE_TEXTURE);

  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, gPosition);

  // for (auto entity : entities_) {
  //   renderQuad();
  // }
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

    // setup plane VAO
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
