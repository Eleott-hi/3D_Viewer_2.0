#pragma once
#include <QOpenGLExtraFunctions>
#include <functional>

namespace s21 {
class TextureWraper : protected QOpenGLExtraFunctions {
 public:
  TextureWraper() {
    initializeOpenGLFunctions();
    Gen();
    Bind();
  }

  ~TextureWraper() { Unbind(); }

  void SetTarget(GLenum target) { m_Target = target; }
  void Gen() { glGenTextures(1, &m_TextureID); }
  uint32_t ID() { return m_TextureID; }

  void SetFormats(GLenum internal_format, GLenum format, GLenum type) {
    m_InternalFormat = internal_format;
    m_Format = format;
    m_Type = type;
  }

  void SetFilters(GLenum min, GLenum mag) {
    m_MinFilter = min;
    m_MagFilter = mag;
  }

  void SetWraps(GLenum wrapS, GLenum wrapT, GLenum wrapR) {
    m_WrapS = wrapS;
    m_WrapT = wrapT;
    m_WrapR = wrapR;
  }

  void Clear() {
    if (m_TextureID) glDeleteTextures(1, &m_TextureID);
    m_TextureID = 0;
  }

  void Bind() { glBindTexture(GL_TEXTURE_2D, m_TextureID); }

  void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

  void Allocate(uint32_t width, uint32_t height, void* data) {
    Q_ASSERT(m_Target != GL_NONE);
    Allocate(m_Target, width, height, data);
  }

  void Allocate(GLenum sub_target, uint32_t width, uint32_t height,
                void* data) {
    Q_ASSERT(sub_target != GL_NONE);
    glTexImage2D(sub_target, 0, m_InternalFormat, width, height, 0, m_Format,
                 m_Type, data);
  }

  void AllocateStorage(uint32_t width, uint32_t height) {
    Q_ASSERT(m_Target != GL_NONE);
    glTexStorage2D(m_Target, 1, m_InternalFormat, width, height);
  }

  void ProcessWrapsAndFilters() {
    Q_ASSERT(m_Target != GL_NONE);

    if (m_WrapS != GL_NONE)
      glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, m_WrapS);
    if (m_WrapT != GL_NONE)
      glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, m_WrapT);
    if (m_WrapR != GL_NONE)
      glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, m_WrapR);

    if (m_MinFilter != GL_NONE || m_MagFilter != GL_NONE) {
      glGenerateMipmap(m_Target);

      if (m_MinFilter != GL_NONE)
        glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, m_MinFilter);
      if (m_MagFilter != GL_NONE)
        glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, m_MagFilter);
    }
  }

 private:
  uint32_t m_TextureID = 0;
  GLenum m_InternalFormat = GL_NONE;
  GLenum m_MinFilter = GL_NONE;
  GLenum m_MagFilter = GL_NONE;
  GLenum m_Target = GL_NONE;
  GLenum m_Format = GL_NONE;
  GLenum m_WrapS = GL_NONE;
  GLenum m_WrapT = GL_NONE;
  GLenum m_WrapR = GL_NONE;
  GLenum m_Type = GL_NONE;
};
}  // namespace s21
