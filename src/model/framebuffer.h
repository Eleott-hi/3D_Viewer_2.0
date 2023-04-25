
// Copyright 2022 pintoved
#pragma once

#include <QOpenGLExtraFunctions>
#include <vector>

#include "i_framebuffer.h"

namespace s21 {

class Framebuffer : public IFramebuffer, protected QOpenGLExtraFunctions {
 public:
  Framebuffer() { initializeOpenGLFunctions(); }

  ~Framebuffer() = default;
  Framebuffer(Framebuffer&&) = default;
  Framebuffer(const Framebuffer&) = default;
  Framebuffer& operator=(Framebuffer&&) = default;
  Framebuffer& operator=(const Framebuffer&) = default;

  virtual void Create(
      const std::initializer_list<AttachmentFormat>& formats) override;
  virtual int ReadPixel(uint x, uint y, int index) override;
  virtual void Resize(uint width, uint height) override;
  virtual uint getTextureID(uint index = 0) override;
  virtual void Bind() override;
  virtual void Unbind() override;

 private:
  void AttachColorTexture(uint index, uint id, GLenum internalFormat,
                          GLenum format);
  void AttachDepthTexture(uint id, GLenum format, GLenum attachmentType);
  void SwitchColorTexture();
  void SwitchDepthTexture();
  void Invalidate();
  void Clear();

 private:
  uint m_fbo = 0;
  uint m_Depth_Texture_ = 0;
  uint width_ = 500, height_ = 500;
  std::vector<uint> m_Color_Textures_;

  std::vector<AttachmentFormat> color_formats_;
  AttachmentFormat depth_format_;
};

}  // namespace s21
