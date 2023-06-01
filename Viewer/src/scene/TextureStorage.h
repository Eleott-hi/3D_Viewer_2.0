#pragma once

#include <QOpenGLExtraFunctions>
#include <string>
#include <unordered_map>

#include "Components.h"

namespace s21 {
class TextureStorage : protected QOpenGLExtraFunctions {
 public:
  static Texture LoadTexture(std::string const& filename);
  static uint32_t LoadCubemap(std::vector<std::string> faces);

 private:
  std::unordered_map<std::string, Texture> textures_;

  ~TextureStorage();
  static TextureStorage& Instance();
  TextureStorage() { initializeOpenGLFunctions(); }
};

}  // namespace s21
