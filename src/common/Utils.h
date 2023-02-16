#pragma once

#include <QMatrix4x4>
#include <QVector3D>

namespace s21::Utils {

QMatrix4x4 setNormalizeMatrix(const QVector3D &min, const QVector3D &max);

template <typename Type>
void append(std::vector<Type> &to, std::vector<Type> &from) {
  to.insert(to.end(), from.begin(), from.end());
}

}  // namespace s21::Utils
