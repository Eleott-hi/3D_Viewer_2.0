
// Copyright 2022 pintoved
#pragma once

#include <QOpenGLExtraFunctions>
#include <vector>

#include "i_framebuffer.h"

namespace s21 {

class Framebuffer : public IFramebuffer, protected QOpenGLExtraFunctions {
 public:
  void CopyBuffersTo(uint32_t to_buffer, uint32_t buffer_mask) final {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, to_buffer);
    glBlitFramebuffer(0, 0, width_, height_,  //
                      0, 0, width_, height_,  //
                      buffer_mask, GL_NEAREST);
  }

  Framebuffer() { initializeOpenGLFunctions(); }

  Framebuffer(Framebuffer&&) = default;
  Framebuffer(const Framebuffer&) = default;
  Framebuffer& operator=(Framebuffer&&) = default;
  Framebuffer& operator=(const Framebuffer&) = default;

  void Clear(uint32_t mask) final { glClear(mask); }
  void ClearColor(QVector4D color) final {
    auto const& [r, g, b, a] = color;
    glClearColor(r, g, b, a);
  }
  void DepthTest(bool enable, uint32_t func = GL_LESS) final {
    enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    glDepthFunc(func);
  }

  void Bind() final { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO_); }
  void Unbind() final { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); }
  uint32_t getTextureID(uint32_t index = 0) final {
    return m_Color_Textures_.at(index);
  }

  void Resize(uint32_t width, uint32_t height) final;
  int ReadPixel(uint32_t x, uint32_t y, int index) final;
  void Create(const std::initializer_list<AttachmentFormat>& formats) final;

 private:
  void Clear();
  void Invalidate();
  void SwitchDepthTexture();
  void SwitchColorTexture();
  void AttachDepthTexture(uint32_t id, GLenum format, GLenum attachmentType);
  void AttachColorTexture(uint32_t index, uint32_t id, GLenum internalFormat,
                          GLenum format);

 private:
  uint32_t FBO_ = 0;
  uint32_t m_Depth_Texture_ = 0;
  uint32_t width_ = 500, height_ = 500;
  std::vector<uint32_t> m_Color_Textures_;

  AttachmentFormat depth_format_;
  std::vector<AttachmentFormat> color_formats_;
};

}  // namespace s21
