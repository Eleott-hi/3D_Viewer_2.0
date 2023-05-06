#ifndef SRC_HEADERS_SIMPLE_TEXTURE_H
#define SRC_HEADERS_SIMPLE_TEXTURE_H

#include "i_technique.h"

namespace s21 {

class SimpleTextureTechnique : public ITechnique {
 public:
  SimpleTextureTechnique() { init(); }
  ~SimpleTextureTechnique() = default;

  void init() final;
  void setTexture(Texture const &texture) final;
  void setMaterial(Material const &material) final;
  void setMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21

#endif  // SRC_HEADERS_SIMPLE_TEXTURE_H
