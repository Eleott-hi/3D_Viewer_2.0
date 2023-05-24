
#include "CubemapSystem.h"

#include <QOpenGLShaderProgram>

#include "Utils.h"
#include "stbi/stb_image.h"

namespace s21 {

void CubemapSystem::irradiance_convolution() {
  uint32_t captureFBO;
  uint32_t captureRBO;
  uint32_t irradianceMap;

  QMatrix4x4 captureProjection;
  captureProjection.perspective(90.0f, 1.0f, 0.1f, 10.0f);

  QOpenGLShaderProgram irradianceShader;
  bool ok = irradianceShader.addShaderFromSourceFile(
      QOpenGLShader::Vertex, ":/shaders/irradiance_convolution.vs");
  Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
             "Failed to compile vertex shader");

  ok = irradianceShader.addShaderFromSourceFile(
      QOpenGLShader::Fragment, ":/shaders/irradiance_convolution.fs");
  Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
             "Failed to compile fragment shader");

  ok = irradianceShader.link();
  Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
             "Failed to link shader program");

  std::vector<QMatrix4x4> captureViews(6);

  QVector3D vecs[6][3] = {
      {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f),
       QVector3D(0.0f, -1.0f, 0.0f)},
      {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f),
       QVector3D(0.0f, -1.0f, 0.0f)},
      {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f),
       QVector3D(0.0f, 0.0f, 1.0f)},
      {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f),
       QVector3D(0.0f, 0.0f, -1.0f)},
      {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f),
       QVector3D(0.0f, -1.0f, 0.0f)},
      {QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f),
       QVector3D(0.0f, -1.0f, 0.0f)}};

  for (size_t i = 0; i < 6; i++)
    captureViews[i].lookAt(vecs[i][0], vecs[i][1], vecs[i][2]);

  glGenFramebuffers(1, &captureFBO);
  glGenRenderbuffers(1, &captureRBO);

  glGenTextures(1, &irradianceMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0,
                 GL_RGB, GL_FLOAT, nullptr);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

  irradianceShader.bind();
  irradianceShader.setUniformValue("environmentMap", 0);
  irradianceShader.setUniformValue("projection", captureProjection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_.id);

  glViewport(0, 0, 32, 32);
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (uint32_t i = 0; i < 6; ++i) {
    irradianceShader.setUniformValue("view", captureViews[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap,
                           0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    RenderCube();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  EntityID entity = scene_->NewEntity();
  scene_->AddComponent<Enviroment>(entity, {{0, "irradianceMap"}});
}

uint32_t CubemapSystem::loadCubemap(std::vector<std::string> faces) {
  uint32_t textureID = 0;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (uint32_t i = 0; i < faces.size(); i++) {
    unsigned char *data =
        stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
      qDebug() << "Cubemap texture failed to load at path:" << faces[i].c_str();
      continue;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}

CubemapSystem::CubemapSystem() { initializeOpenGLFunctions(); }

void CubemapSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void CubemapSystem::Update() {
  glDepthFunc(GL_LEQUAL);

  for (auto entity : entities_) {
    auto const &[proj, view] = Utils::GetProjectionAndView(scene_);
    auto &texture = scene_->GetComponent<Texture>(entity);
    auto &mesh = scene_->GetComponent<Mesh>(entity);

    technique_->Enable(TechniqueType::CUBEMAP);
    technique_->setMVP(proj, view, {});
    technique_->setTexture(texture);

    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
  }

  glDepthFunc(GL_LESS);
}

}  // namespace s21
