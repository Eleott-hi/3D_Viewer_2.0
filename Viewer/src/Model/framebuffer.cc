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

  SwitchColorTexture();
  SwitchDepthTexture();

  Invalidate();
}

void Framebuffer::Clear() {
  if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
  m_fbo = 0;
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
  return textures_.at(index).ID();
}

void Framebuffer::Invalidate() {
  if (m_fbo) Clear();

  glGenFramebuffers(1, &m_fbo);
  Bind();

  ProcessTextures();

  Q_ASSERT_X(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "Framebuffer::Init()", "Framebuffer incomplete");

  GLenum buf[] = {
      GL_COLOR_ATTACHMENT0,
      GL_COLOR_ATTACHMENT1,
      GL_COLOR_ATTACHMENT2,
  };

  glDrawBuffers(3, buf);

  Unbind();
}

void Framebuffer::ProcessTextures() {
  Q_ASSERT(textures_.size());

  for (auto &texture : textures_) {
    texture.Clear();
    texture.Gen();
    texture.Bind();

    // texture.SetBorderColor({1.0, 1.0, 1.0, 1.0});
    texture.ProcessWrapsAndFilters();
    texture.AllocateStorage(width_, height_);

    glFramebufferTexture2D(GL_FRAMEBUFFER, texture.Attachment(),
                           texture.Target(), texture.ID(), 0);

    texture.Unbind();
  }
}

void Framebuffer::SwitchColorTexture() {
  TextureWraper texture(GL_TEXTURE_2D);

  for (uint32_t i = 0; i < color_formats_.size(); i++) {
    switch (color_formats_[i].format_) {
      case Format::RGB:
        texture.SetFormats(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);
        break;
      case Format::RGBA:
        texture.SetFormats(GL_RGBA8, GL_RGB, GL_UNSIGNED_BYTE);
        break;
      case Format::RGBA16F:
        texture.SetFormats(GL_RGBA16F, GL_RGBA, GL_FLOAT);
        break;
      case Format::RED_INTEGER:
        texture.SetFormats(GL_R32I, GL_RED_INTEGER, GL_UNSIGNED_BYTE);
        break;
      default:
        Q_ASSERT_X(false, "Framebuffer::Invalidate()", "No such color format");
        break;
    }

    texture.SetWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture.SetFilters(GL_LINEAR, GL_LINEAR);
    texture.SetAttachment(GL_COLOR_ATTACHMENT0 + i);

    textures_.push_back(texture);
  }
}

void Framebuffer::SwitchDepthTexture() {
  TextureWraper texture(GL_TEXTURE_2D);

  switch (depth_format_.format_) {
    case Format::DEPTH24_STENCIL8:
      texture.SetFormats(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL,
                         GL_UNSIGNED_INT_24_8);
      texture.SetAttachment(GL_DEPTH_STENCIL_ATTACHMENT);
      break;

    case Format::DEPTH32:
      texture.SetFormats(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
      texture.SetAttachment(GL_DEPTH_ATTACHMENT);
      break;

    case Format::NONE:
      qDebug() << "Framebuffer::SwitchDepthTexture()"
               << "depth format none";
      break;

    default:
      Q_ASSERT_X(false, "Framebuffer::Invalidate()", "No such depth format");
      break;
  }

  texture.SetWraps(GL_CLAMP_TO_BORDER);
  texture.SetFilters(GL_NEAREST);

  textures_.push_back(texture);
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
