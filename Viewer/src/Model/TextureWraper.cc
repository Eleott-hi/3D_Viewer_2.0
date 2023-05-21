#include "TextureWraper.h"

namespace s21 {

TextureWraper::TextureWraper(GLenum target) : m_Target(target) {
  initializeOpenGLFunctions();
}

void TextureWraper::Gen() { glGenTextures(1, &m_TextureID); }

uint32_t TextureWraper::ID() { return m_TextureID; }
void TextureWraper::SetFilters(GLenum filter) { SetFilters(filter, filter); }
void TextureWraper::SetWraps(GLenum wrap) { SetWraps(wrap, wrap, wrap); }
void TextureWraper::SetInternalFormat(GLenum internal_format) {
  m_InternalFormat = internal_format;
}
void TextureWraper::SetFormat(GLenum format) { m_Format = format; }
void TextureWraper::SetType(GLenum type) { m_Type = type; }

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

void TextureWraper::Bind() {
  Q_ASSERT(m_Target != GL_NONE), glBindTexture(m_Target, m_TextureID);
}

void TextureWraper::Unbind() {
  Q_ASSERT(m_Target != GL_NONE), glBindTexture(m_Target, 0);
}

void TextureWraper::Allocate(uint32_t width, uint32_t height, void* data) {
  Q_ASSERT(m_Target != GL_NONE);
  Allocate(m_Target, width, height, data);
}

void TextureWraper::Allocate(GLenum sub_target, uint32_t width, uint32_t height,
                             void* data) {
  Q_ASSERT(sub_target != GL_NONE);
  glTexImage2D(sub_target, 0, m_InternalFormat, width, height, 0, m_Format,
               m_Type, data);
}

void TextureWraper::AllocateStorage(uint32_t width, uint32_t height) {
  Q_ASSERT(m_Target != GL_NONE);
  glTexStorage2D(m_Target, 1, m_InternalFormat, width, height);
}

void TextureWraper::ProcessWrapsAndFilters() {
  Q_ASSERT(m_Target != GL_NONE);

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
