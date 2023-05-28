
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
  // uint32_t getDepthID() final { return m_Depth_Texture_; }
  void Resize(uint32_t width, uint32_t height, bool invalidate = true) final;
  int ReadPixel(uint32_t x, uint32_t y, int index) final;
  void Create(const std::initializer_list<AttachmentFormat>& formats) final;
  void SetStartCoordinates(uint32_t x, uint32_t y) final { x_ = x, y_ = y; }
  void PrepereBuffer() final;

  void SetPrepereBuffer(std::function<void()> prepare_func) final {
    prepare_func_ = prepare_func;
  }

  void AddTexture(TextureWraper const& texture) final {
    textures_.push_back(texture);
  }

 private:
  void Clear();
  void Invalidate();
  void SwitchDepthTexture();
  void SwitchColorTexture();
  void ProcessTextures();

 private:
  uint32_t m_fbo = 0;
  std::function<void()> prepare_func_;
  uint32_t x_ = 0, y_ = 0;
  uint32_t width_ = 500, height_ = 500;

  std::vector<TextureWraper> textures_;

  uint32_t m_Depth_Texture_ = 0;
  std::vector<uint32_t> m_Color_Textures_;

  bool depth_storage_ = true;
  AttachmentFormat depth_format_;
  std::vector<AttachmentFormat> color_formats_;
};

}  // namespace s21
