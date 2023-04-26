#include <QApplication>

#include "backend.h"
#include "controller.h"
#include "main_window.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QSurfaceFormat format;
  format.setVersion(4, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);

  s21::Backend model;
  s21::Controller controller(&model);
  s21::MainWindow window(&controller);

  window.show();

  return app.exec();
}
