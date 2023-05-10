#pragma once

#include <QMatrix4x4>
#include <QVector3D>

#include "core/ECS_Controller.h"

namespace s21::Utils {

template <typename Type>
void append(std::vector<Type>& to, std::vector<Type>& from) {
  to.insert(to.end(), from.begin(), from.end());
}

EntityID GetQuad(ECS_Controller* scene);
EntityID GetCameraID(ECS_Controller* scene);
static EntityID GetProjectionID(ECS_Controller* scene);
std::string ArrayName(std::string const& name, int index);
QMatrix4x4 setNormalizeMatrix(const QVector3D& min, const QVector3D& max);
std::string StructName(std::string const& struct_name, std::string const& name);
void SetVector(QVector3D& vec, float const& x, float const& y, float const& z);
std::tuple<QMatrix4x4, QMatrix4x4> GetProjectionAndView(ECS_Controller* scene);

}  // namespace s21::Utils
