#pragma once

#include <QMatrix4x4>
#include <QVector3D>

#include "core/ECS_Controller.h"

namespace s21::Utils {

QMatrix4x4 setNormalizeMatrix(const QVector3D& min, const QVector3D& max);

template <typename Type>
void append(std::vector<Type>& to, std::vector<Type>& from) {
  to.insert(to.end(), from.begin(), from.end());
}

void SetVector(QVector3D& vec, float const& x, float const& y, float const& z);

static EntityID GetCameraID(ECS_Controller* scene);
static EntityID GetProjectionID(ECS_Controller* scene);
std::tuple<QMatrix4x4, QMatrix4x4> GetProjectionAndView(ECS_Controller* scene);

}  // namespace s21::Utils
