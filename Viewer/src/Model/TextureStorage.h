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

  Texture loadTexture(std::string const& filename);

 private:
  std::unordered_map<std::string, Texture> textures_;
};

}  // namespace s21