#include "Utils.h"

#include "Components.h"

namespace s21::Utils {

EntityID GetCamera(ECS_Controller* scene) {
  static auto cameras = scene->GetEntities<Camera>();

  Q_ASSERT(cameras.size() == 1);

  return cameras[0];
}

// static EntityID GetProjectionID(ECS_Controller* scene) {
//   auto projections = scene->GetEntities<Projection>();

//   Q_ASSERT(projections.size() == 1);
  
//   return projections[0];
// }

// std::tuple<QMatrix4x4, QMatrix4x4> GetProjectionAndView(ECS_Controller* scene) {
//   // static EntityID camera_id = GetCameraID(scene);
//   static EntityID projection_id = GetProjectionID(scene);

//   auto const& projection = scene->GetComponent<Projection>(projection_id);
//   auto const& camera = scene->GetComponent<Camera>(camera_id);

//   return {camera.projection_matrix, camera.view_matrix};
// }

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

EntityID GetQuad(ECS_Controller* scene) {
  static EntityID quad = scene->GetEntities<QuadTag>()[0];

  return quad;
}

EntityID GetTimer(ECS_Controller* scene) {
  static auto tick_time = scene->GetEntities<Timer>();

  Q_ASSERT(tick_time.size() == 1);

  return tick_time[0];
}

EntityID GetInput(ECS_Controller* scene) {
  static auto input = scene->GetEntities<Input>();

  Q_ASSERT(input.size() == 1);

  return input[0];
}

std::string StructName(std::string const& struct_name,
                       std::string const& name) {
  return (struct_name + "." + name).c_str();
}

std::string ArrayName(std::string const& name, int index) {
  return (name + "[" + std::to_string(index) + "]").c_str();
}

}  // namespace s21::Utils
