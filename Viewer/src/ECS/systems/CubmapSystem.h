
#pragma once

#include <QKeyEvent>
#include <QPoint>
#include <QVector3D>
#include <unordered_map>

#include "Components.h"
#include "core/ECS_Controller.h"
#include "core/Event.h"
#include "core/System.h"

namespace s21 {

class CubmapeSystem : public System, protected QOpenGLExtraFunctions {
 public:
  CubmapeSystem() = default;
  ~CubmapeSystem() = default;

  void Update();
  void Init(ECS_Controller *scene);

 private:
  ECS_Controller *scene_ = nullptr;
  TechniqueStrategy *technique_ = nullptr;
};
}  // namespace s21
