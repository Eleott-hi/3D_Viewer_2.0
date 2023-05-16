#include "ShadowSystem.h"

#include "Utils.h"

namespace s21 {

int SHADOW_WIDTH = 1024;
int SHADOW_HEIGHT = 1024;

QMatrix4x4 Projection() {
  QMatrix4x4 m;
  float near_plane = 1.0f, far_plane = 1000.f;
  m.ortho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);

  return m;
}

QMatrix4x4 View() {
  QMatrix4x4 m;
  QVector3D lightPos(-2.0f, 4.0f, -1.0f);

  m.lookAt(lightPos, QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0));

  return m;
}

void ShadowSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;

  glGenFramebuffers(1, &depthMapFBO);

  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
               SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float borderColor[] = {0.5, 0.5, 0.5, 1.0};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthMap, 0);

  Q_ASSERT_X(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,  //
      "ShadowSystem::Init()",                                               //
      "ShadowSystem incomplete");

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowSystem::Update() {
  // static auto proj = Projection();
  // static auto view = View();

  auto const &[proj, view] = Utils::GetProjectionAndView(scene_);

  // proj = Projection();

  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);

    technique_->Enable(TechniqueType::SHADOW_MAPPING);
    technique_->setMVP(proj, view, transform.GetModelMatrix());

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}  // namespace s21