
#include "types.h"

namespace s21 {
ComponentID GetComponentID() {
  static ComponentID componentCounter = 0;

  assert(componentCounter < MAX_COMPONENTS &&
         "GetComponentID: Component limits achieved");

  return componentCounter++;
}

}  // namespace s21