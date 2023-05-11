#pragma once

#include <QOpenGLExtraFunctions>
#include <string>
#include <unordered_map>

#include "Components.h"

namespace s21 {
class TextureStorage : public QOpenGLExtraFunctions {
 public:
  TextureStorage() { initializeOpenGLFunctions(); }
  ~TextureStorage() = default;

  uint32_t LoadTexture(std::string const& filename);
  uint32_t LoadCubemap(std::vector<std::string> faces);

 private:
  std::unordered_map<std::string, uint32_t> textures_;
};

}  // namespace s21