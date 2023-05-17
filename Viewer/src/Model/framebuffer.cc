#include "framebuffer.h"

#include "TextureWraper.h"

namespace s21 {

#ifdef __WIN32__
const float magicScale_ = 1.25;
#elif __APPLE__
const float magicScale_ = 2;
#else
const float magicScale_ = 1;
#endif

namespace Utils {

static bool isDepth(Format f) { return f >= Format::NONE; }
static void GenTexture(uint32_t size, uint32_t *id) { glGenTextures(size, id); }
static void BindTexture(uint32_t id) { glBindTexture(GL_TEXTURE_2D, id); }
static void UnbindTexture() { BindTexture(0); }

}  // namespace Utils

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

void Framebuffer::Resize(uint32_t width, uint32_t height) {
  uint32_t w = width * magicScale_;
  uint32_t h = height * magicScale_;

  if (width_ == w && height_ == h) return;

  width_ = w, height_ = h;
  Invalidate();
}

void Framebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }
void Framebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

uint32_t Framebuffer::getTextureID(uint32_t index) {
  return m_Color_Textures_.at(index);
}

void Framebuffer::Invalidate() {
  if (m_fbo) Clear();

  glGenFramebuffers(1, &m_fbo);
  Bind();

  SwitchDepthTexture();
  SwitchColorTexture();

  Q_ASSERT_X(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "Framebuffer::Init()", "Framebuffer incomplete");

  if (!m_Color_Textures_.empty()) {
#define ARRAY_SIZE(x) ((sizeof(x)) / (sizeof(x[0])))

    GLenum buf[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
    };
    int n = ARRAY_SIZE(buf);

    Q_ASSERT_X(m_Color_Textures_.size() <= n,  //
               "Framebuffer::Invalidate()",    //
               "not supported quantity of attachments");

    glDrawBuffers(n, buf);
#undef ARRAY_SIZE

  } else {
    glDrawBuffer(GL_NONE);
  }

  Unbind();
}

void Framebuffer::SwitchColorTexture() {
  for (uint32_t i = 0; i < color_formats_.size(); i++) {
    switch (color_formats_[i].format_) {
      case Format::RGB:
        AttachColorTexture(i, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);
        break;
      case Format::RGBA:
        AttachColorTexture(i, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        break;
      case Format::RGBA16F:
        AttachColorTexture(i, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        break;
      case Format::RED_INTEGER:
        AttachColorTexture(i, GL_R32I, GL_RED_INTEGER, GL_UNSIGNED_BYTE);
        break;
      default:
        Q_ASSERT_X(false, "Framebuffer::Invalidate()", "No such color format");
        break;
    }
  }
}

void Framebuffer::AttachColorTexture(uint32_t index, GLenum internalFormat,
                                     GLenum format, GLenum type) {
  TextureWraper texture;
  texture.SetTarget(GL_TEXTURE_2D);
  texture.SetFormats(internalFormat, format, type);
  texture.SetWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  texture.SetFilters(GL_LINEAR, GL_LINEAR);
  texture.ProcessWrapsAndFilters();
  texture.AllocateStorage(width_, height_);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                         GL_TEXTURE_2D, texture.ID(), 0);

  m_Color_Textures_.push_back(texture.ID());
}

void Framebuffer::SwitchDepthTexture() {
  switch (depth_format_.format_) {
    case Format::DEPTH24_STENCIL8:
      AttachDepthTexture(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL,
                         GL_UNSIGNED_INT_24_8, GL_DEPTH_STENCIL_ATTACHMENT);
      break;

    case Format::DEPTH32:
      AttachDepthTexture(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT,
                         GL_DEPTH_ATTACHMENT);
      break;

    case Format::NONE:
      qDebug() << "Framebuffer::SwitchDepthTexture()"
               << "depth format none";
      break;

    default:
      Q_ASSERT_X(false, "Framebuffer::Invalidate()", "No such depth format");
      break;
  }
}

void Framebuffer::AttachDepthTexture(GLenum internal_format, GLenum format,
                                     GLenum type, GLenum attachmentType) {
  TextureWraper texture;
  texture.SetTarget(GL_TEXTURE_2D);
  texture.SetFormats(internal_format, format, type);
  texture.SetWraps(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
  texture.SetFilters(GL_NEAREST, GL_NEAREST);
  texture.ProcessWrapsAndFilters();
  
  float borderColor[] = {1.0, 1.0, 1.0, 1.0};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  texture.Allocate(width_, height_, nullptr);

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D,
                         texture.ID(), 0);

  m_Depth_Texture_ = texture.ID();
}

int Framebuffer::ReadPixel(uint32_t x, uint32_t y, int index) {
  Q_ASSERT_X(index <= m_Color_Textures_.size(), "Framebuffer::ReadPixel()",
             "Index is out of bounds");

  Bind();
  glReadBuffer(GL_COLOR_ATTACHMENT0 + index);

  int objectID = 0;

  glReadPixels(x * magicScale_, height_ - y * magicScale_, 1, 1, GL_RED_INTEGER,
               GL_INT, &objectID);

  glReadBuffer(GL_NONE);
  Unbind();
  return objectID;
}
};  // namespace s21
