
// Copyright 2022 pintoved
#pragma once

#include <QOpenGLExtraFunctions>
#include <vector>

#include "i_framebuffer.h"

namespace s21 {

class Framebuffer : public IFramebuffer, protected QOpenGLExtraFunctions {
 public:
  Framebuffer() { initializeOpenGLFunctions(); }

  // ~Framebuffer() {glDeleteFramebuffers(1, &m_fbo);  };
  Framebuffer(Framebuffer&&) = default;
  Framebuffer(const Framebuffer&) = default;
  Framebuffer& operator=(Framebuffer&&) = default;
  Framebuffer& operator=(const Framebuffer&) = default;

  virtual void Create(
      const std::initializer_list<AttachmentFormat>& formats) override;
  virtual int ReadPixel(uint32_t x, uint32_t y, int index) override;
  virtual void Resize(uint32_t width, uint32_t height) override;
  virtual uint32_t getTextureID(uint32_t index = 0) override;
  virtual void Bind() override;
  virtual void Unbind() override;

 private:
  void AttachColorTexture(uint32_t index, uint32_t id, GLenum internalFormat,
                          GLenum format);
  void AttachDepthTexture(uint32_t id, GLenum format, GLenum attachmentType);
  void SwitchColorTexture();
  void SwitchDepthTexture();
  void Invalidate();
  void Clear();

 private:
  uint32_t m_fbo = 0;
  uint32_t m_Depth_Texture_ = 0;
  uint32_t width_ = 500, height_ = 500;
  std::vector<uint32_t> m_Color_Textures_;

  std::vector<AttachmentFormat> color_formats_;
  AttachmentFormat depth_format_;
};

}  // namespace s21
