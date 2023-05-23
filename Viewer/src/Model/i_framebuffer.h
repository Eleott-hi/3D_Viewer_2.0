
// Copyright 2022 pintoved

#ifndef I_FRAMEBUFFER_H
#define I_FRAMEBUFFER_H

#include "TextureWraper.h"

namespace s21 {

enum class Format {
  // Color
  RGB,
  RGBA,
  RGBA16F,
  RED_INTEGER,

  NONE,

  // Depth
  DEPTH32,
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
  virtual uint32_t getTextureID(uint32_t index = 0) = 0;
  virtual void AddTexture(TextureWraper const& texture) = 0;
  virtual void Resize(uint32_t width, uint32_t height,
                      bool invalidate = true) = 0;
  virtual int ReadPixel(uint32_t x, uint32_t y, int index = 0) = 0;
  virtual void PrepereBuffer() = 0;
  virtual void SetPrepereBuffer(std::function<void()> prepare_func) = 0;
  virtual void Create(
      const std::initializer_list<AttachmentFormat>& formats) = 0;
};

}  // namespace s21
#endif  // I_FRAMEBUFFER_H
