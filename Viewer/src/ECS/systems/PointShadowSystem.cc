
#include "PointShadowSystem.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
float near_plane = 1;
float far_plane = 25;

namespace s21 {

QMatrix4x4 ProjPoint() {
  QMatrix4x4 m;

  m.perspective(qDegreesToRadians(90),
                (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane,
                far_plane);

  return m;
}

std::vector<QMatrix4x4> ProjViewPoint(QVector3D lightPos) {
  std::vector<QMatrix4x4> looks(6);
  std::vector<QMatrix4x4> views;

  auto shadowProj = ProjPoint();

  looks[0].lookAt(lightPos, lightPos + QVector3D(1, 0, 0), QVector3D(0, -1, 0));
  looks[1].lookAt(lightPos, lightPos + QVector3D(-1, 0, 0),
                  QVector3D(0, -1, 0));
  looks[2].lookAt(lightPos, lightPos + QVector3D(0, 1, 0), QVector3D(0, 0, 1));
  looks[3].lookAt(lightPos, lightPos + QVector3D(0, -1, 0),
                  QVector3D(0, 0, -1));
  looks[4].lookAt(lightPos, lightPos + QVector3D(0, 0, 1), QVector3D(0, -1, 0));
  looks[5].lookAt(lightPos, lightPos + QVector3D(0, 0, -1),
                  QVector3D(0, -1, 0));

  std::vector<QMatrix4x4> shadowTransforms = {
      shadowProj * looks[0], shadowProj * looks[1], shadowProj * looks[2],
      shadowProj * looks[3], shadowProj * looks[4], shadowProj * looks[5],
  };

  return views;
}

void PointShadowSystem::Init(ECS_Controller *scene,
                             TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;

  glGenFramebuffers(1, &depthMapFBO);
  glGenTextures(1, &depthCubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
  for (unsigned int i = 0; i < 6; ++i)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  // attach depth texture as FBO's depth buffer
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);

  Q_ASSERT_X(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "PointShadowSystem::Init()", "Framebuffer incomplete");

  // glDrawBuffer(GL_NONE);
  // glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointShadowSystem::Update() {
  static auto lightPos = QVector3D(0, 0, 2);
  static auto matriceis = ProjViewPoint(lightPos);

  // glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);

  technique_->Enable(TechniqueType::POINT_SHADOW);

  for (unsigned int i = 0; i < matriceis.size(); ++i)
    technique_->setCustomValue(("shadowMatrices[" + std::to_string(i) + "]").c_str(),
                               matriceis.at(i));

  technique_->setCustomValue("far_plane", far_plane);
  technique_->setCustomValue("lightPos", lightPos);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);

    technique_->setCustomValue("model", transform.GetModelMatrix());
    // technique_->setCustomValue("model", transform.GetModelMatrix());
    // technique_->setMVP(proj, view, transform.GetModelMatrix());

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
