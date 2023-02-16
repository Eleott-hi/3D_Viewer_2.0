#include "TextureStorage.h"

#include "stb_image.h"

namespace s21 {

TextureStorage::TextureStorage() {  //
  initializeOpenGLFunctions();
  stbi_set_flip_vertically_on_load(true);
}

uint32_t TextureStorage::LoadTexture(std::string const &filename) {
  if (textureIDs_.find(filename) != textureIDs_.end()) {
    qDebug() << "Texture already loaded at path: " << filename.c_str();
    return textureIDs_.at(filename);
  }

  uint32_t textureID = LoadFromFile(filename);
  textureIDs_.insert({filename, textureID});

  return textureID;
}

uint32_t TextureStorage::LoadFromFile(std::string const &filename) {
  uint32_t textureID = 0;
  int width, height, nrComponents;

  unsigned char *data =
      stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

  if (!data) {
    qDebug() << "Texture failed to load at path: " << filename.c_str();
    return textureID;
  }

  GLenum format;
  if (nrComponents == 1)
    format = GL_RED;
  else if (nrComponents == 3)
    format = GL_RGB;
  else if (nrComponents == 4)
    format = GL_RGBA;

  glGenTextures(1, &textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  qDebug() << nrComponents;

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  return textureID;
}

}  // namespace s21
