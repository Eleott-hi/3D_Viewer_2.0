#include "TextureStorage.h"

#include "stb_image.h"

namespace s21 {

TextureStorage::TextureStorage() {
  stbi_set_flip_vertically_on_load(true);
  initializeOpenGLFunctions();
}

Texture TextureStorage::loadTexture(std::string const& filename) {
  if (textures_[filename].id) return textures_[filename];

  int width, height, nrComponents;
  unsigned char* data =
      stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

  if (!data) {
    qDebug() << "Texture failed to load at path: " << filename.c_str();
    return {};
  }

  GLenum format;
  if (nrComponents == 1)
    format = GL_RED;
  else if (nrComponents == 3)
    format = GL_RGB;
  else if (nrComponents == 4)
    format = GL_RGBA;

  Texture texture;
  // uint32_t id = 0;
  glGenTextures(1, &texture.id);

  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  textures_[filename] = texture;

  return texture;
}
}  // namespace s21
