#pragma once
#include <QOpenGLExtraFunctions>
#include <vector>

enum class TextureTemplate {
  RGB,
  RGBA,
  RGBA16F,
  RED_INTEGER,

  DEPTH32,
  DEPTH24_STENCIL8,

};

namespace s21 {
class TextureWraper : protected QOpenGLExtraFunctions {
 public:
  TextureWraper() { initializeOpenGLFunctions(); }
  TextureWraper(GLenum target) : m_Target(target) {
    initializeOpenGLFunctions();
  }
  TextureWraper(GLenum target, TextureTemplate t);

  ~TextureWraper() = default;

  GLenum Target() { return m_Target; }
  uint32_t ID() { return m_TextureID; }
  GLenum Attachment() { return m_Attachment; }
  void SetType(GLenum type) { m_Type = type; }
  void SetFormat(GLenum format) { m_Format = format; }
  void SetWraps(GLenum wrap) { SetWraps(wrap, wrap, wrap); }
  void SetFilters(GLenum filter) { SetFilters(filter, filter); }
  void SetAttachment(GLenum attachment) { m_Attachment = attachment; }
  void SetInternalFormat(GLenum internal_format) {
    m_InternalFormat = internal_format;
  }
  void Allocate(uint32_t width, uint32_t height, void* data) {
    Allocate(m_Target, width, height, data);
  }

  void Gen();
  void Bind();
  void Clear();
  void Unbind();
  void ProcessWrapsAndFilters();
  void SetFilters(GLenum min, GLenum mag);
  void AllocateStorage(uint32_t width, uint32_t height);
  void SetWraps(GLenum wrapS, GLenum wrapT, GLenum wrapR);

  void SetBorderColor(std::vector<float> const& borderColor);
  void SetFormats(GLenum internal_format, GLenum format, GLenum type);
  void Allocate(GLenum sub_target, uint32_t width, uint32_t height, void* data);

 private:
  uint32_t m_TextureID = 0;

  GLenum m_Type = GL_NONE;
  GLenum m_WrapR = GL_NONE;
  GLenum m_WrapS = GL_NONE;
  GLenum m_WrapT = GL_NONE;
  GLenum m_Format = GL_NONE;
  GLenum m_Target = GL_NONE;
  GLenum m_MinFilter = GL_NONE;
  GLenum m_MagFilter = GL_NONE;
  GLenum m_Attachment = GL_NONE;
  GLenum m_InternalFormat = GL_NONE;
};
}  // namespace s21
