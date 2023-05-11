#include "TextureStorage.h"

#include "stb_image.h"

namespace s21 {

struct ImageInfo {
  std::string filename;
  int width;
  int height;
  int channels;
};

unsigned char* LoadImage(ImageInfo& info, bool flip_vertically = false) {
  if (flip_vertically) stbi_set_flip_vertically_on_load(true);

  auto& [path, width, height, channels] = info;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  stbi_set_flip_vertically_on_load(false);

  return data;
}

GLenum GetFormat(int channels) {
  if (channels == 1) return GL_RED;
  if (channels == 3) return GL_RGB;
  if (channels == 4) return GL_RGBA;

  Q_ASSERT(false);
}

uint32_t TextureStorage::LoadTexture(std::string const& filename) {
  if (textures_[filename]) return textures_[filename];

  ImageInfo info = {filename, 0, 0, 0};

  auto data = LoadImage(info, true);

  if (!data) {
    qDebug() << "Texture failed to load at path: " << filename.c_str();
    return 0;
  }

  uint32_t texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GetFormat(info.channels), info.width,
               info.height, 0, GetFormat(info.channels), GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  textures_[filename] = texture;

  qDebug() << filename.c_str();

  return texture;
}

uint32_t TextureStorage::LoadCubemap(std::vector<std::string> faces) {
  uint32_t textureID = 0;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (uint32_t i = 0; i < faces.size(); i++) {
    unsigned char* data =
        stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
      qDebug() << "Cubemap texture failed to load at path:" << faces[i].c_str();
      continue;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}

}  // namespace s21
