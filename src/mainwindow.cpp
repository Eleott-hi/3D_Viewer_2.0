#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->AxisLines->setAutoExclusive(false);
  ui->Perspective->setAutoExclusive(false);
  ui->SmoothPoints->setAutoExclusive(false);
  ui->DashedLines->setAutoExclusive(false);
  ui->Filename->setWordWrap(true);

  gifTimer = new QTimer();
  connect(gifTimer, SIGNAL(timeout()), this, SLOT(gif_timer()));

  setUpSettings();
}

void MainWindow::rememberSettings() {
  QSettings settings(QDir::homePath() + "/3D_Viewer_1.0/3D_Viewer_v1.plist",
                     QSettings::NativeFormat);

  // Remember filename
  settings.setValue("ModelFilename", ui->Filename->text());

  // Remember translations, scale
  settings.setValue("xTrans", ui->xTransDoubleBox->value());
  settings.setValue("yTrans", ui->yTransDoubleBox->value());
  settings.setValue("zTrans", ui->zTransDoubleBox->value());
  settings.setValue("Scale", ui->ScaleDoubleBox->value());

  // Remember perspective, axis, points and lines settings
  settings.setValue("ShowVertex", vertex);
  settings.setValue("PointSize", ui->PointSize->value());
  settings.setValue("LineWidth", ui->LineWidth->value());
  settings.setValue("SmoothPoints", ui->SmoothPoints->isChecked());
  settings.setValue("DashedLines", ui->DashedLines->isChecked());
  settings.setValue("AxisLines", ui->AxisLines->isChecked());
  settings.setValue("Perspective", ui->Perspective->isChecked());

  // Remember colors settings
  settings.setValue("BackgroundColor", backgroundColor);
  settings.setValue("ModelColor", modelColor);

  float **rotationMatrix = ui->openGLWidget->getRotationMatrix();
  for (int i = 0; i < 16; ++i)
    settings.setValue(QString::number(i), *(rotationMatrix[0] + i));
}

void MainWindow::setUpSettings() {
  if (QFile::exists(QDir::homePath() + "/3D_Viewer_1.0/3D_Viewer_v1.plist")) {
    QSettings settings(QDir::homePath() + "/3D_Viewer_1.0/3D_Viewer_v1.plist",
                       QSettings::NativeFormat);

    // Set up filename
    openFile(settings.value("ModelFilename",
             QDir::homePath() + "/3D_Viewer_1.0/models/091_W_Aya_100K.obj").toString());

    // Set up translations, scale
    ui->xTransDoubleBox->setValue(settings.value("xTrans", ui->xTransDoubleBox->value()).toDouble());
    ui->yTransDoubleBox->setValue(settings.value("yTrans", ui->yTransDoubleBox->value()).toDouble());
    ui->zTransDoubleBox->setValue(settings.value("zTrans", ui->zTransDoubleBox->value()).toDouble());
    ui->ScaleDoubleBox->setValue(settings.value("Scale",   ui->ScaleDoubleBox ->value()).toDouble());


    // Set up perspective, axis, points and lines settings
    if (!settings.value("ShowVertex", true).toBool()) ui->vertex_show->click();
    ui->PointSize->   setValue(settings.value("PointSize",      ui->PointSize   ->value()).toInt());
    ui->LineWidth->   setValue(settings.value("LineWidth",      ui->LineWidth   ->value()).toInt());
    ui->SmoothPoints->setChecked(settings.value("SmoothPoints", ui->SmoothPoints->isChecked()).toBool());
    ui->DashedLines-> setChecked(settings.value("DashedLines",  ui->DashedLines ->isChecked()).toBool());
    ui->AxisLines->   setChecked(settings.value("AxisLines",    ui->AxisLines   ->isChecked()).toBool());
    ui->Perspective-> setChecked(settings.value("Perspective",  ui->Perspective ->isChecked()).toBool());

    // Set up  colors settings
    backgroundColor = settings.value("BackgroundColor").value<QColor>();
    modelColor = settings.value("ModelColor").value<QColor>();

    float **rotationMatrix = ui->openGLWidget->getRotationMatrix();
    for (int i = 0; i < 16; ++i)
      *(rotationMatrix[0] + i) = settings.value(QString::number(i)).toFloat();
  } else {
    backgroundColor.setRgbF(1, 1, 1, 1);  // white color
    modelColor.setRgbF(0.7, 0.7, 0.7, 1); // gray color
    openFile(QDir::homePath() + "/3D_Viewer_1.0/models/091_W_Aya_100K.obj");
  }

  ui->openGLWidget->setColor(true, backgroundColor.redF(),
                                   backgroundColor.greenF(),
                                   backgroundColor.blueF());
  ui->openGLWidget->setColor(false, modelColor.redF(),
                                    modelColor.greenF(),
                                    modelColor.blueF());
  ui->openGLWidget->update();
}

MainWindow::~MainWindow() {
  rememberSettings();
  delete gifTimer;
  delete ui;
}

void MainWindow::on_File_clicked() {
  QString filename = QFileDialog::getOpenFileName(
      nullptr, "Choose file", QDir::homePath() + "/3D_Viewer_1.0/models");
  openFile(filename);
}

void MainWindow::openFile(QString filename) {
  if (!filename.isEmpty()) {
    ui->Filename->setText(filename.toStdString().c_str());
    ui->openGLWidget->setModelData(filename.toStdString().c_str());

    std::ostringstream verteces;
    std::ostringstream facets;
    verteces << ui->openGLWidget->getVertexCount();
    facets << ui->openGLWidget->getFacetCount();
    ui->VertecesLabel->setText(verteces.str().c_str());
    ui->FacetsLabel->setText(facets.str().c_str());
    ui->openGLWidget->update();
  }
}

void MainWindow::on_Screenshot_clicked() {
  QImage screenshot =
      ui->openGLWidget->grabFramebuffer(); // получаем картинку из openGLWIDGET
  const QString format = "jpeg";
  QString default_path = "images";
  QFileDialog save_dialog(this, tr("Save as"), default_path);
  save_dialog.setAcceptMode(QFileDialog::AcceptSave);
  save_dialog.setFileMode(QFileDialog::AnyFile);
  save_dialog.setDirectory(default_path);
  save_dialog.setDefaultSuffix(format);

  QStringList mime_types;
  const QList<QByteArray> ba_mime_types = QImageWriter::supportedMimeTypes();

  for (const QByteArray &bf : ba_mime_types)
    mime_types.append(QLatin1String(bf));
  save_dialog.setMimeTypeFilters(mime_types);
  save_dialog.selectMimeTypeFilter("image/" + format);

  if (save_dialog.exec() == QDialog::Accepted) {
    const QString fname = save_dialog.selectedFiles().first();
    if (!screenshot.save(fname)) {
      QMessageBox::warning(this, tr("Error"),
                           tr("Imposible to save image in \"%1\".")
                               .arg(QDir::toNativeSeparators(fname)));
    }
  }
}

void MainWindow::on_Gif_clicked() {
  if (gifStart == 0) {
    gifStart = 1;
    ui->Gif->setText("Save");
    record();
  } else {
    ui->Gif->setText("Gif");
    gifStart = 0;
    gifTimer->stop();
    save();
  }
}

void MainWindow::record() {
  gifOut = new QGifImage();
  gifTimer->start(100);
}

void MainWindow::save() {
  gifTime = 0;
  gifStart = 0;
  QString safeGIF = QFileDialog::getSaveFileName(this, "Сохранить как...", "",
                                                 "GIF Files (*.gif)");
  if (!safeGIF.isNull())
    gifOut->save(safeGIF);
  gifOut->~QGifImage();
}

void MainWindow::gif_timer() {

  qDebug() << gifTime++;

  QImage frame = ui->openGLWidget->grabFramebuffer();
  frame = frame.scaled(640, 480, Qt::IgnoreAspectRatio);
  gifOut->addFrame(frame, 100);
}

void MainWindow::on_xTrans_valueChanged(int value) {
  ui->xTransDoubleBox->setValue((double)value);
}

void MainWindow::on_yTrans_valueChanged(int value) {
  ui->yTransDoubleBox->setValue((double)value);
}

void MainWindow::on_zTrans_valueChanged(int value) {
  ui->zTransDoubleBox->setValue((double)value);
}

void MainWindow::on_xRot_valueChanged(int value) {
  ui->xRotDoubleBox->setValue((double)value);
}

void MainWindow::on_yRot_valueChanged(int value) {
  ui->yRotDoubleBox->setValue((double)value);
}

void MainWindow::on_zRot_valueChanged(int value) {
  ui->zRotDoubleBox->setValue((double)value);
}

void MainWindow::on_Scale_valueChanged(int value) {
  ui->ScaleDoubleBox->setValue((double)value);
}

void MainWindow::on_PointSize_valueChanged(int value) {
  ui->openGLWidget->setPointSize(value);
  ui->openGLWidget->update();
}

void MainWindow::on_LineWidth_valueChanged(int value) {
  ui->openGLWidget->setLineWidth(value);
  ui->openGLWidget->update();
}

void MainWindow::on_xTransDoubleBox_valueChanged(double value) {
  ui->xTrans->setValue((int)value);
  ui->openGLWidget->setXTrans(1.0 / M_PI * value);
  ui->openGLWidget->update();
}

void MainWindow::on_yTransDoubleBox_valueChanged(double value) {
  ui->yTrans->setValue((int)value);
  ui->openGLWidget->setYTrans(1.0 / M_PI * value);
  ui->openGLWidget->update();
}

void MainWindow::on_zTransDoubleBox_valueChanged(double value) {
  ui->zTrans->setValue((int)value);
  ui->openGLWidget->setZTrans(1.0 / M_PI * value);
  ui->openGLWidget->update();
}

void MainWindow::on_xRotDoubleBox_valueChanged(double value) {
  xRotEnd = value;
  ui->openGLWidget->setXRot(20.0 / M_PI * (value - xRotStart));
  ui->xRot->setValue((int)value);
  ui->openGLWidget->update();
}

void MainWindow::on_yRotDoubleBox_valueChanged(double value) {
  ui->yRot->setValue((int)value);
  yRotEnd = value;
  ui->openGLWidget->setYRot(20.0 / M_PI * (value - yRotStart));
  ui->openGLWidget->update();
}

void MainWindow::on_zRotDoubleBox_valueChanged(double value) {
  zRotEnd = value;
  ui->openGLWidget->setZRot(20.0 / M_PI * (value - zRotStart));
  ui->zRot->setValue((int)value);
  ui->openGLWidget->update();
}

void MainWindow::on_ScaleDoubleBox_valueChanged(double value) {
  ui->openGLWidget->setXScale(value);
  ui->openGLWidget->setYScale(value);
  ui->openGLWidget->setZScale(value);
  ui->Scale->setValue((int)value >= 100 ? 100 : (int)value);
  ui->openGLWidget->update();
}

void MainWindow::on_Reset_clicked() {
  ui->Scale->setValue(50);
  ui->Scale->setValue(1);
  ui->xTrans->setValue(0);
  ui->yTrans->setValue(0);
  ui->zTrans->setValue(-2 * M_PI);

  ui->xRot->setValue(0);
  ui->yRot->setValue(0);
  ui->zRot->setValue(0);
  ui->openGLWidget->initRotationMatrix();

  ui->PointSize->setValue(1);
  ui->LineWidth->setValue(1);
  ui->openGLWidget->update();
}

void MainWindow::on_Perspective_toggled(bool checked) {
  ui->openGLWidget->setPerspective(checked);
  ui->openGLWidget->setXRot(0);
  ui->openGLWidget->setYRot(0);
  ui->openGLWidget->setZRot(0);
  ui->openGLWidget->update();
}

void MainWindow::on_AxisLines_toggled(bool checked) {
  ui->openGLWidget->setAxisLines(checked);
  ui->openGLWidget->update();
}

void MainWindow::on_SmoothPoints_toggled(bool checked) {
  ui->openGLWidget->setPointForm(checked);
  ui->openGLWidget->update();
}

void MainWindow::on_DashedLines_toggled(bool checked) {
  ui->openGLWidget->setLinesForm(checked);
  ui->openGLWidget->update();
}

void MainWindow::on_BackgroundColor_clicked() {
  backgroundColor = QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (backgroundColor.isValid()) {
    _setColor(true, backgroundColor.redF(), backgroundColor.greenF(),
              backgroundColor.blueF());
  }
}

void MainWindow::on_ModelColor_clicked() {
  modelColor = QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (modelColor.isValid()) {
    _setColor(false, modelColor.redF(), modelColor.greenF(),
              modelColor.blueF());
  }
}

void MainWindow::_setColor(bool background, float r, float g, float b) {

  ui->openGLWidget->setColor(background, r, g, b);
  ui->openGLWidget->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control) {
    ui->openGLWidget->setCtrl(true);
  } else if (event->key() == Qt::Key_Escape) {
    this->close();
  } else if (event->key() == Qt::Key_D) {
    ui->xTransDoubleBox->setValue(ui->xTransDoubleBox->value() + 1);
  } else if (event->key() == Qt::Key_A) {
    ui->xTransDoubleBox->setValue(ui->xTransDoubleBox->value() - 1);
  } else if (event->key() == Qt::Key_W) {
    ui->yTransDoubleBox->setValue(ui->yTransDoubleBox->value() + 1);

  } else if (event->key() == Qt::Key_S) {
    ui->yTransDoubleBox->setValue(ui->yTransDoubleBox->value() - 1);
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control) {
    ui->openGLWidget->setCtrl(false);
  }
}

void MainWindow::on_xRot_sliderReleased() {
  xRotStart = xRotEnd;
  ui->openGLWidget->remember_rotation_matrix();
}

void MainWindow::on_yRot_sliderReleased() {
  yRotStart = yRotEnd;
  ui->openGLWidget->remember_rotation_matrix();
}

void MainWindow::on_zRot_sliderReleased() {
  zRotStart = zRotEnd;
  ui->openGLWidget->remember_rotation_matrix();
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  if (event->angleDelta().y() > 0) {
    ui->zTransDoubleBox->setValue(ui->zTransDoubleBox->value() + 1);
  } else if (event->angleDelta().y() < 0) {
    ui->zTransDoubleBox->setValue(ui->zTransDoubleBox->value() - 1);
  }
}

void MainWindow::on_vertex_show_clicked() {
  if (vertex) {
    vertex = 0;
    ui->openGLWidget->setdots(vertex);
    ui->vertex_show->setText("Show Vertex");
    ui->openGLWidget->update();
  } else {
    vertex = 1;
    ui->openGLWidget->setdots(vertex);
    ui->vertex_show->setText("Hide Vertex");
    ui->openGLWidget->update();
  }
}
