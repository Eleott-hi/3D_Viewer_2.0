#include <QApplication>

#include "Backend.h"
#include "MainWindow.h"
#include <QSurfaceFormat>

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
  s21::MainWindow window(&model);

  window.show();

  return app.exec();
}
