#pragma once
#include <QOpenGLExtraFunctions>
#include <functional>
#include <vector>

namespace s21 {
class TextureWraper : protected QOpenGLExtraFunctions {
 public:
  TextureWraper(GLenum target);
  ~TextureWraper() = default;

  uint32_t ID() { return m_TextureID; }
  uint32_t Target() { return m_Target; }
  void SetType(GLenum type) { m_Type = type; }
  void Unbind() { glBindTexture(m_Target, 0); }
  void Gen() { glGenTextures(1, &m_TextureID); }
  void SetFormat(GLenum format) { m_Format = format; }
  void Bind() { glBindTexture(m_Target, m_TextureID); }
  void SetWraps(GLenum wrap) { SetWraps(wrap, wrap, wrap); }
  void SetFilters(GLenum filter) { SetFilters(filter, filter); }
  void SetInternalFormat(GLenum internal_format) {
    m_InternalFormat = internal_format;
  }
  void Allocate(uint32_t width, uint32_t height, void* data) {
    Allocate(m_Target, width, height, data);
  }

  void Clear();
  void ProcessWrapsAndFilters();
  void SetFilters(GLenum min, GLenum mag);
  void AllocateStorage(uint32_t width, uint32_t height);
  void SetWraps(GLenum wrapS, GLenum wrapT, GLenum wrapR);

  void SetBorderColor(std::vector<float> const& borderColor);
  void SetFormats(GLenum internal_format, GLenum format, GLenum type);
  void Allocate(GLenum sub_target, uint32_t width, uint32_t height, void* data);

 private:
  GLenum m_InternalFormat = GL_NONE;
  GLenum m_MinFilter = GL_NONE;
  GLenum m_MagFilter = GL_NONE;
  GLenum m_Target = GL_NONE;
  GLenum m_Format = GL_NONE;
  GLenum m_WrapS = GL_NONE;
  GLenum m_WrapT = GL_NONE;
  GLenum m_WrapR = GL_NONE;
  GLenum m_Type = GL_NONE;

  uint32_t m_TextureID = 0;
};
}  // namespace s21
