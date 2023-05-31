#include "ShaderProgram.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

namespace s21 {

QString ProcessFile(QString filename) {
  QFileInfo fileInfo(filename);
  QString directory = fileInfo.absoluteDir().absolutePath();

  qDebug() << fileInfo;

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) Q_ASSERT(false);

  QString res;

  while (!file.atEnd()) {
    QString line = file.readLine();

    if (line.contains("import")) {
      QString str;
      QTextStream ss(&line);
      bool start_import = false;

      while (!ss.atEnd()) {
        ss >> str;
        qDebug() << str;

        if (str.contains("import")) {
          start_import = true;
          continue;
        }

        if (start_import) {
          res += ProcessFile(directory + "/" + str);
        }
      }

    } else {
      res += line;
    }

    res += "\n";
  }

  return res;
}

void ShaderProgram::GenerateShaders(QString vertex_file, QString fragment_file,
                                    QString geometry_file) {
  bool ok = shader_.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                            ProcessFile(vertex_file));
  Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
             "Failed to compile vertex shader");

  ok = shader_.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       ProcessFile(fragment_file));
  Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
             "Failed to compile fragment shader");

  if (geometry_file != "") {
    ok = shader_.addShaderFromSourceCode(QOpenGLShader::Geometry,
                                         ProcessFile(geometry_file));
    Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
               "Failed to compile geometry shader");
  }

  ok = shader_.link();
  Q_ASSERT_X(ok, "ITechnique::GenerateShaders",
             "Failed to link shader program");
}

}  // namespace s21
