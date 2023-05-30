#pragma once

#include <QOpenGLExtraFunctions>
#include <string>
#include <unordered_map>

#include "Components.h"

namespace s21 {
class TextureStorage : public QOpenGLExtraFunctions {
 public:
  TextureStorage() { initializeOpenGLFunctions(); }
  ~TextureStorage();

  Texture LoadTexture(std::string const& filename);
  uint32_t LoadCubemap(std::vector<std::string> faces);

 private:
  std::unordered_map<std::string, Texture> textures_;
};

}  // namespace s21
