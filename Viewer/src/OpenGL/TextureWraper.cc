#include "TextureWraper.h"

#include "OpenGLDebug.h"

namespace s21 {

void TextureWraper::Unbind() { OPENGL_DEBUG(glBindTexture(m_Target, 0)); }
void TextureWraper::Gen() { OPENGL_DEBUG(glGenTextures(1, &m_TextureID)); }
void TextureWraper::Bind() {
  OPENGL_DEBUG(glBindTexture(m_Target, m_TextureID));
}

TextureWraper::TextureWraper(GLenum target, TextureTemplate t)
    : m_Target(target) {
  initializeOpenGLFunctions();
  switch (t) {
    case TextureTemplate::RGB:
      SetFormats(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);
      break;
    case TextureTemplate::RGBA:
      SetFormats(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
      break;

    case TextureTemplate::RGBA16F:
      SetFormats(GL_RGBA16F, GL_RGBA, GL_FLOAT);
      break;

    case TextureTemplate::RED_INTEGER:
      SetFormats(GL_R32I, GL_RED_INTEGER, GL_UNSIGNED_BYTE);
      break;
  }
}

void TextureWraper::SetBorderColor(std::vector<float> const& borderColor) {
  OPENGL_DEBUG(
      glTexParameterfv(m_Target, GL_TEXTURE_BORDER_COLOR, borderColor.data()));
}

void TextureWraper::SetFormats(GLenum internal_format, GLenum format,
                               GLenum type) {
  m_Type = type;
  m_Format = format;
  m_InternalFormat = internal_format;
}

void TextureWraper::SetFilters(GLenum min, GLenum mag) {
  m_MinFilter = min;
  m_MagFilter = mag;
}

void TextureWraper::SetWraps(GLenum wrapS, GLenum wrapT, GLenum wrapR) {
  m_WrapS = wrapS;
  m_WrapT = wrapT;
  m_WrapR = wrapR;
}

void TextureWraper::Clear() {
  if (m_TextureID) {
    OPENGL_DEBUG(glDeleteTextures(1, &m_TextureID));
    m_TextureID = 0;
  }
}

void TextureWraper::Allocate(GLenum sub_target, uint32_t width, uint32_t height,
                             void* data) {
  Q_ASSERT(sub_target != GL_NONE);
  OPENGL_DEBUG(glTexImage2D(sub_target, 0, m_InternalFormat, width, height, 0,
                            m_Format, m_Type, data));
}

void TextureWraper::AllocateStorage(uint32_t width, uint32_t height) {
  if (m_Target == GL_TEXTURE_CUBE_MAP) {
    width = height = std::max(width, height);
    qDebug() << "TextureWraper::AllocateStorage()" << width << height;
  }

  OPENGL_DEBUG(glTexStorage2D(m_Target, 1, m_InternalFormat, width, height));
}

void TextureWraper::ProcessWrapsAndFilters() {
  if (m_WrapS != GL_NONE)
    OPENGL_DEBUG(glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, m_WrapS));
  if (m_WrapT != GL_NONE)
    OPENGL_DEBUG(glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, m_WrapT));
  if (m_WrapR != GL_NONE)
    OPENGL_DEBUG(glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, m_WrapR));

  if (m_MinFilter == GL_NONE && m_MagFilter == GL_NONE) return;

  OPENGL_DEBUG(glGenerateMipmap(m_Target));
  if (m_MinFilter != GL_NONE)
    OPENGL_DEBUG(glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, m_MinFilter));
  if (m_MagFilter != GL_NONE)
    OPENGL_DEBUG(glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, m_MagFilter));
}

}  // namespace s21
