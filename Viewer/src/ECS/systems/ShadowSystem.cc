#include "ShadowSystem.h"

#include "Utils.h"

namespace s21 {
QMatrix4x4 Proj() {
  float near_plane = 1.0f, far_plane = 100.0f;
  QMatrix4x4 m;

  m.ortho(-100.0, 100.0, -100.0, 100.0, near_plane, far_plane);

  return m;
}

QMatrix4x4 View() {
  QMatrix4x4 m;
  // QVector3D lightPos = QVector3D(-100.0f, 100.0f, -1.0f);

  QVector3D lightPos = QVector3D(-10.0f, 10.0f, -1.0f);
  m.lookAt(lightPos, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0, 1.0, 0.0));

  return m;
}

void ShadowSystem::Init(ECS_Controller *scene, TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;
}

void ShadowSystem::Update() {
  // auto const &[proj, view_] = Utils::GetProjectionAndView(scene_);

  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));

  static auto proj = Proj();
  static auto view = View();

  for (auto entity : entities_) {
    auto &model = scene_->GetComponent<Mesh>(entity);
    auto const &transform = scene_->GetComponent<Transform>(entity);

    technique_->Enable(TechniqueType::SHADOW_MAPPING);
    technique_->SetMVP(camera.projection_matrix, camera.view_matrix,
                       transform.GetModelMatrix());

    model.Draw(this, GL_TRIANGLES);
  }
}

}  // namespace s21
