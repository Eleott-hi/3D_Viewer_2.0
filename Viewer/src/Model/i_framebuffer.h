
// Copyright 2022 pintoved

#ifndef I_FRAMEBUFFER_H
#define I_FRAMEBUFFER_H
#include <QVector4D>
namespace s21 {

enum class Format {
  NONE,

  // Color
  RGB,
  RGBA,
  RGBA16F,
  RED_INTEGER,

  // Depth
  DEPTH24_STENCIL8,

  // Default
  DEFAULT_COLOR = RGB,
  DEFAULT_DEPTH = DEPTH24_STENCIL8
};

struct AttachmentFormat {
  AttachmentFormat() = default;
  AttachmentFormat(Format format) : format_(format) {}

  Format format_ = Format::NONE;
};

class IFramebuffer {
 public:
  IFramebuffer() = default;
  virtual ~IFramebuffer() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;
  virtual void Clear(uint32_t mask) = 0;
  virtual void CopyBuffersTo(uint32_t to_buffer, uint32_t buffer_mask) = 0;
  virtual void ClearColor(QVector4D color) = 0;
  virtual uint32_t getTextureID(uint32_t index = 0) = 0;
  virtual void Resize(uint32_t width, uint32_t height) = 0;
  virtual void DepthTest(bool enable, uint32_t func = GL_LESS) = 0;
  virtual int ReadPixel(uint32_t x, uint32_t y, int index = 0) = 0;
  virtual void Create(
      const std::initializer_list<AttachmentFormat> &formats) = 0;
};

}  // namespace s21
#endif  // I_FRAMEBUFFER_H
