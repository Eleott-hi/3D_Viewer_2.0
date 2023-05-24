#include <QApplication>

#include "Scene.h"
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


  s21::Scene scene;
  s21::MainWindow window(&scene);

  window.show();

  return app.exec();
}
