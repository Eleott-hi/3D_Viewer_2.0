#ifndef SRC_HEADERS_TEXTURE_STORAGE_H
#define SRC_HEADERS_TEXTURE_STORAGE_H

#include <QOpenGLExtraFunctions>
#include <map>

#include "Components.h"

namespace s21 {

class TextureStorage : protected QOpenGLExtraFunctions {
 public:
  TextureStorage();
  ~TextureStorage() = default;

  uint32_t LoadTexture(std::string const &filename);

 private:
  std::map<std::string, uint32_t> textureIDs_;

  uint32_t LoadFromFile(std::string const &filename);
};

}  // namespace s21

#endif  // SRC_HEADERS_TEXTURE_STORAGE_H
