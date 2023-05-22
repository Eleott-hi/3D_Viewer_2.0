#include <QDebug>

void ClearErrors();

uint32_t glCheckErrors(const char *file, int line);

#define OPENGL_DEBUG(x) \
  ClearErrors();        \
  (x);                  \
  glCheckErrors(__FILE__, __LINE__);
