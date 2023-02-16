#include "framebuffer.h"

namespace s21 {

// =============================== UTILS ===============================
namespace Utils {
static bool isDepth(Format f) { return (f == Format::DEPTH24_STENCIL8); }
static void GenTexture(uint size, uint *id) { glGenTextures(size, id); }
static void BindTexture(uint id) { glBindTexture(GL_TEXTURE_2D, id); }
static void UnbindTexture() { BindTexture(0); }
}  // namespace Utils

// =============================== UTILS ===============================

void Framebuffer::Create(
    const std::initializer_list<AttachmentFormat> &formats) {
  color_formats_.clear();
  for (auto &format : formats) {
    if (Utils::isDepth(format.format_)) {
      depth_format_ = format;
    } else {
      color_formats_.push_back(format);
    }
  }
  Invalidate();
}

void Framebuffer::Clear() {
  if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
  if (!m_Color_Textures_.empty())
    glDeleteTextures(m_Color_Textures_.size(), m_Color_Textures_.data());
  if (m_Depth_Texture_) glDeleteTextures(1, &m_Depth_Texture_);

  m_Color_Textures_.clear();
  m_Depth_Texture_ = 0;
}

void Framebuffer::Resize(uint width, uint height) {
  float magicScale = 1.25;
  width_ = width * magicScale;
  height_ = height * magicScale;
  Invalidate();
}

void Framebuffer::Bind() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo); }
void Framebuffer::Unbind() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); }

uint Framebuffer::getTextureID(uint index) {
  return m_Color_Textures_.at(index);
}

// =============================== INVALIDATE ===============================
void Framebuffer::Invalidate() {
  if (m_fbo) Clear();

  // Gen framebuffer
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  // Gen & Attach depth texture
  if (depth_format_.format_ != Format::NONE) {
    Utils::GenTexture(1, &m_Depth_Texture_);
    SwitchDepthTexture();
  }

  // Gen & Attach color texture
  if (!color_formats_.empty()) {
    m_Color_Textures_.resize(color_formats_.size());
    Utils::GenTexture(m_Color_Textures_.size(), m_Color_Textures_.data());
    SwitchColorTexture();
  }

  // Verify that the FBO is correct
  Q_ASSERT_X(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "Framebuffer::Init()", "Framebuffer incomplete");

  if (!m_Color_Textures_.empty()) {
#define ARRAY_SIZE(x) ((sizeof(x)) / (sizeof(x[0])))

    GLenum buf[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    int n = ARRAY_SIZE(buf);

    Q_ASSERT_X(m_Color_Textures_.size() <= n, "Framebuffer::Invalidate()",
               "not supported quantity of attachments");

    glDrawBuffers(n, buf);
#undef ARRAY_SIZE

  } else {
    glDrawBuffer(GL_NONE);
  }

  // Restore the default framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// ========================== ATTACH COLOR TEXTURE ==========================
void Framebuffer::SwitchColorTexture() {
  for (uint i = 0; i < m_Color_Textures_.size(); i++) {
    switch (color_formats_[i].format_) {
      case Format::RGB:
        AttachColorTexture(i, m_Color_Textures_[i], GL_RGB, GL_RGB);
        break;
      case Format::RED_INTEGER:
        AttachColorTexture(i, m_Color_Textures_[i], GL_R32I, GL_RED_INTEGER);
        break;
      default:
        Q_ASSERT_X(false, "Framebuffer::Invalidate()", "No such color format");
        break;
    }
  }
}

void Framebuffer::AttachColorTexture(uint index, uint id, GLenum internalFormat,
                                     GLenum format) {
  glBindTexture(GL_TEXTURE_2D, id);

  // Set depth texture
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width_, height_, 0, format,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Attach color texture to framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                         GL_TEXTURE_2D, id, 0);

  glBindTexture(GL_TEXTURE_2D, 0);
}

// ========================== ATTACH DEPTH TEXTURE ==========================
void Framebuffer::SwitchDepthTexture() {
  switch (depth_format_.format_) {
    case Format::DEPTH24_STENCIL8:
      AttachDepthTexture(m_Depth_Texture_, GL_DEPTH24_STENCIL8,
                         GL_DEPTH_STENCIL_ATTACHMENT);
      break;
    default:
      Q_ASSERT_X(false, "Framebuffer::Invalidate()", "No such depth format");
      break;
  }
}

void Framebuffer::AttachDepthTexture(uint id, GLenum format,
                                     GLenum attachmentType) {
  Utils::BindTexture(id);

  // Set depth texture
  glTexStorage2D(GL_TEXTURE_2D, 1, format, width_, height_);

  // Attach depth texture to framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0);

  Utils::UnbindTexture();
}

// =============================== READ BUFFER ===============================
int Framebuffer::ReadPixel(uint x, uint y, int index) {
  Q_ASSERT_X(index <= m_Color_Textures_.size(), "Framebuffer::ReadPixel()",
             "Index is out of bounds");

  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
  glReadBuffer(GL_COLOR_ATTACHMENT0 + index);

  float magicScale = 1.25;
  int objectID = 0;

  glReadPixels(x * magicScale, height_ - y * magicScale, 1, 1, GL_RED_INTEGER,
               GL_INT, &objectID);

  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  return objectID;
}
};  // namespace s21
