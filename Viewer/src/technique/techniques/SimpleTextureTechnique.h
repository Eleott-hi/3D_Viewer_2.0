#ifndef SRC_HEADERS_SIMPLE_TEXTURE_H
#define SRC_HEADERS_SIMPLE_TEXTURE_H

#include "ITechnique.h"

namespace s21 {

class SimpleTextureTechnique : public ITechnique {
 public:
  SimpleTextureTechnique() { Init(); }
  ~SimpleTextureTechnique() = default;

  void Init() final;
  void SetTexture(Texture const &texture) final;
  void SetMaterial(Material const &material) final;
  void SetMVP(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 model) final;
};

}  // namespace s21

#endif  // SRC_HEADERS_SIMPLE_TEXTURE_H
