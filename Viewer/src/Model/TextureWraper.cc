#include "TextureWraper.h"

namespace s21 {

TextureWraper::TextureWraper(GLenum target) : m_Target(target) {
  initializeOpenGLFunctions();
}

void TextureWraper::SetBorderColor(std::vector<float> const& borderColor) {
  glTexParameterfv(m_Target, GL_TEXTURE_BORDER_COLOR, borderColor.data());
}

void TextureWraper::SetFormats(GLenum internal_format, GLenum format,
                               GLenum type) {
  m_InternalFormat = internal_format;
  m_Format = format;
  m_Type = type;
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
  if (m_TextureID) glDeleteTextures(1, &m_TextureID);
  m_TextureID = 0;
}

void TextureWraper::Allocate(GLenum sub_target, uint32_t width, uint32_t height,
                             void* data) {
  Q_ASSERT(sub_target != GL_NONE);
  glTexImage2D(sub_target, 0, m_InternalFormat, width, height, 0, m_Format,
               m_Type, data);
}

void TextureWraper::AllocateStorage(uint32_t width, uint32_t height) {
  glTexStorage2D(m_Target, 1, m_InternalFormat, width, height);
}

void TextureWraper::ProcessWrapsAndFilters() {
  if (m_WrapS != GL_NONE) glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, m_WrapS);
  if (m_WrapT != GL_NONE) glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, m_WrapT);
  if (m_WrapR != GL_NONE) glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, m_WrapR);

  if (m_MinFilter != GL_NONE || m_MagFilter != GL_NONE) {
    glGenerateMipmap(m_Target);

    if (m_MinFilter != GL_NONE)
      glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, m_MinFilter);
    if (m_MagFilter != GL_NONE)
      glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, m_MagFilter);
  }
}

}  // namespace s21
