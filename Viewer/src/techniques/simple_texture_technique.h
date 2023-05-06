#ifndef SRC_HEADERS_SIMPLE_TEXTURE_H
#define SRC_HEADERS_SIMPLE_TEXTURE_H

#include "i_technique.h"

namespace s21 {

class SimpleTextureTechnique : public ITechnique {
 public:
  SimpleTextureTechnique() { init(); }
  ~SimpleTextureTechnique() = default;

  virtual void init() override;
  virtual void setTextureID(uint32_t id) override;
  virtual void setMVP(QMatrix4x4 proj, QMatrix4x4 view,
                      QMatrix4x4 model) override;
};

}  // namespace s21

#endif  // SRC_HEADERS_SIMPLE_TEXTURE_H
