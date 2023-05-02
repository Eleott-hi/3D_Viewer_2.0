#include "Utils.h"

#include "Components.h"

namespace s21::Utils {

static EntityID GetCameraID(ECS_Controller* scene) {
  auto cameras = scene->GetEntities<Camera>();
  Q_ASSERT(cameras.size() == 1);
  return cameras[0];
}

static EntityID GetProjectionID(ECS_Controller* scene) {
  auto projections = scene->GetEntities<Projection>();
  Q_ASSERT(projections.size() == 1);
  return projections[0];
}

std::tuple<QMatrix4x4, QMatrix4x4> GetProjectionAndView(ECS_Controller* scene) {
  static EntityID camera_id = GetCameraID(scene);
  static EntityID projection_id = GetProjectionID(scene);

  auto const& projection = scene->GetComponent<Projection>(projection_id);
  auto const& camera = scene->GetComponent<Camera>(camera_id);

  return {projection.matrix, camera.GetViewMatrix()};
}

QMatrix4x4 setNormalizeMatrix(const QVector3D& min, const QVector3D& max) {
  QVector3D CenterTranslate = {(min.x() + max.x()) / 2.0f,
                               (min.y() + max.y()) / 2.0f,
                               (min.z() + max.z()) / 2.0f};
  std::vector<float> scales = {2.0f / (max.x() - min.x()),
                               2.0f / (max.y() - min.y()),
                               2.0f / (max.z() - min.z())};
  QMatrix4x4 m;

  m.scale(*(std::min_element(scales.begin(), scales.end())));
  m.translate(-CenterTranslate);

  return m;
}

void SetVector(QVector3D& vec, float const& x, float const& y, float const& z) {
  vec.setX(x);
  vec.setY(y);
  vec.setZ(z);
}

}  // namespace s21::Utils
