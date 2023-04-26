#include "Utils.h"

#include <QOpenGLExtraFunctions>

namespace s21::Utils {

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
