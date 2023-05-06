#pragma once

#include <QOpenGLExtraFunctions>
#include <string>
#include <unordered_map>

#include "Components.h"

namespace s21 {
class TextureStorage : public QOpenGLExtraFunctions {
 public:
  TextureStorage();
  ~TextureStorage() = default;

  uint32_t loadTexture(std::string const& filename);

 private:
  std::unordered_map<std::string, uint32_t> textures_;
};

}  // namespace s21