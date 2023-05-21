#pragma once
#include <QOpenGLExtraFunctions>
#include <functional>

namespace s21 {
class TextureWraper : protected QOpenGLExtraFunctions {
 public:
  TextureWraper(GLenum target);
  ~TextureWraper() = default;

  void Gen();
  void Bind();
  void Clear();
  void Unbind();
  uint32_t ID();
  void SetType(GLenum type);
  void SetWraps(GLenum wrap);
  void SetFormat(GLenum format);
  void ProcessWrapsAndFilters();
  void SetFilters(GLenum filter);
  void SetFilters(GLenum min, GLenum mag);
  void SetInternalFormat(GLenum internal_format);
  void AllocateStorage(uint32_t width, uint32_t height);
  void SetWraps(GLenum wrapS, GLenum wrapT, GLenum wrapR);
  void Allocate(uint32_t width, uint32_t height, void* data);
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
