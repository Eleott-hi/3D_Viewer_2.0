#pragma once

#include "core/ECS_Controller.h"

namespace s21 {

class Parser {
 public:
  static void loadModel(ECS_Controller *scene, std::string const &filename);
};

}  // namespace s21
