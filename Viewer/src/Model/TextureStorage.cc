#include "TextureStorage.h"

#include "TextureWraper.h"
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

Texture TextureStorage::LoadTexture(std::string const& filename) {
  if (textures_[filename].id) return textures_[filename];

  ImageInfo info = {filename, 0, 0, 0};

  auto data = LoadImage(info, true);

  if (!data) {
    qDebug() << "Texture failed to load at path: " << filename.c_str();
    return {};
  }

  auto format = GetFormat(info.channels);

  TextureWraper texture(GL_TEXTURE_2D);
  texture.Gen();
  texture.Bind();
  texture.SetFilters(GL_LINEAR, GL_LINEAR);
  texture.SetWraps(GL_REPEAT, GL_REPEAT, GL_REPEAT);
  texture.SetFormats(format, format, GL_UNSIGNED_BYTE);
  texture.Allocate(info.width, info.height, data);
  texture.ProcessWrapsAndFilters();
  texture.Unbind();

  stbi_image_free(data);

  textures_[filename].id = texture.ID();
  textures_[filename].filename = filename.c_str();
  textures_[filename].image = QImage(filename.c_str());

  qDebug() << filename.c_str();

  return textures_[filename];
}

uint32_t TextureStorage::LoadCubemap(std::vector<std::string> faces) {
  TextureWraper texture(GL_TEXTURE_CUBE_MAP);
  texture.Gen();
  texture.Bind();
  texture.SetWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  texture.SetFilters(GL_LINEAR, GL_LINEAR);
  texture.ProcessWrapsAndFilters();

  for (uint32_t i = 0; i < faces.size(); i++) {
    int width, height, channels;

    unsigned char* data =
        stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

    if (!data) {
      qDebug() << "Cubemap texture failed to load at path:" << faces[i].c_str();
      continue;
    }

    auto format = GetFormat(channels);
    texture.SetFormats(format, format, GL_UNSIGNED_BYTE);
    texture.Allocate(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width, height, data);
    stbi_image_free(data);
  }

  texture.Unbind();

  return texture.ID();
}

}  // namespace s21
