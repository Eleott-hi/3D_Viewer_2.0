
// Copyright 2022 pintoved
#pragma once

#include <QOpenGLExtraFunctions>
#include <vector>

#include "i_framebuffer.h"

namespace s21 {

class Framebuffer : public IFramebuffer, protected QOpenGLExtraFunctions {
 public:
  Framebuffer() { initializeOpenGLFunctions(); }

  Framebuffer(Framebuffer&&) = default;
  Framebuffer(const Framebuffer&) = default;
  Framebuffer& operator=(Framebuffer&&) = default;
  Framebuffer& operator=(const Framebuffer&) = default;

  void Bind() final;
  void Unbind() final;
  uint32_t getTextureID(uint32_t index = 0) final;
  uint32_t getDepthID() final { return m_Depth_Texture_; }
  void Resize(uint32_t width, uint32_t height) final;
  int ReadPixel(uint32_t x, uint32_t y, int index) final;
  void Create(const std::initializer_list<AttachmentFormat>& formats) final;
  void PrepereBuffer() final {
    Q_ASSERT(prepare_func_);
    prepare_func_();
  }

  void SetPrepereBuffer(std::function<void()> prepare_func) final {
    prepare_func_ = prepare_func;
  }

 private:
  void Clear();
  void Invalidate();
  void SwitchDepthTexture();
  void SwitchColorTexture();
  void AttachDepthTexture(GLenum internal_format, GLenum format, GLenum type,
                          GLenum attachmentType);
  void AttachColorTexture(uint32_t index, GLenum internalFormat, GLenum format,
                          GLenum type);

 private:
  uint32_t m_fbo = 0;
  uint32_t m_Depth_Texture_ = 0;
  uint32_t width_ = 500, height_ = 500;
  std::vector<uint32_t> m_Color_Textures_;
  std::function<void()> prepare_func_;

  bool depth_storage_ = true;
  AttachmentFormat depth_format_;
  std::vector<AttachmentFormat> color_formats_;
};

}  // namespace s21
