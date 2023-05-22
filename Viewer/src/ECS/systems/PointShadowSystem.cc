
#include "PointShadowSystem.h"

#include "OpenGLDebug.h"

const float near_plane = 1;
const float far_plane = 25;

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

  return shadowTransforms;
}

void PointShadowSystem::Init(ECS_Controller *scene,
                             TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void PointShadowSystem::Update() {
  static auto lightPos = QVector3D(0, 0, 2);
  static auto matriceis = ProjViewPoint(lightPos);

  technique_->Enable(TechniqueType::POINT_SHADOW);

  for (unsigned int i = 0; i < matriceis.size(); ++i){
    technique_->setCustomValue(
        ("shadowMatrices[" + std::to_string(i) + "]").c_str(), matriceis.at(i));
        qDebug()<< matriceis.at(i);
        }

  technique_->setCustomValue("far_plane", far_plane);
  technique_->setCustomValue("lightPos", lightPos);

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Model>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);

    technique_->setCustomValue("model", transform.GetModelMatrix());

    for (auto &mesh : model.meshes) mesh.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
