#include "glwidget.h"

#include <QDir>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), modelData() {
  // Initializing auxiliary matrices
  matrixIdentity = setIdentityMatrix();
  matrixNormalized = setIdentityMatrix();
  matrixRotationCurrent = setIdentityMatrix();
  matrixRotationPrevios = setIdentityMatrix();
}

GLWidget::~GLWidget() {
  // Deleting auxiliary matrices & modelDataStruct
//   remove_model_data(&modelData);
  s21_remove_matrix(&matrixIdentity);
  s21_remove_matrix(&matrixNormalized);
  s21_remove_matrix(&matrixRotationCurrent);
  s21_remove_matrix(&matrixRotationPrevios);
}

void GLWidget::setModelData(const char *filename) {
  // Parsing .obj file
  modelData.loadFromFile(filename);  
  setNormalizedMatrix();
}

void GLWidget::initializeGL() {
  // Initializing OpenGL functions & general settings
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glShadeModel(GL_SMOOTH);
  glLineStipple(4, 0xAAAA);

  // Initializing shader program
  shader = new Shader(QDir::homePath().toStdString() +
                      "/3D_Viewer_1.0/shaders.txt");  // MacOS
}

void GLWidget::resizeGL(int w, int h) {
  // Update OpenGL Window size port
  width = w;
  height = h;
}

void GLWidget::paintGL() {
  // Main painting function; called by Update() and Resize()
  glClearColor(bRed, bGreen, bBlue, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (axisLines) drawCoordinates();
  drawModel();
}

void GLWidget::drawModel() {
//   if (modelData.vertexes.matrix) {
  if (!modelData.isEmpty()) {
    shader->Bind();  // Binding shader program to set up our settings
    refreshSettings(true);

    glEnableClientState(GL_VERTEX_ARRAY);  // Permission to bind client vertex
                                           // array to graphics card memory
    // glVertexPointer(3, GL_FLOAT, 0, modelData.vertexes.matrix[0]);  // Binding
    glVertexPointer(3, GL_FLOAT, 0, modelData.getVerteces().data());  // Binding
    if (show_vertex)
    //   glDrawArrays(GL_POINTS, 1, modelData.count_of_vertexes);  // Draw dots
      glDrawArrays(GL_POINTS, 1, modelData.getVerteces().size()/3);  // Draw dots
    // for (unsigned i = 0; i < modelData.count_of_facets; ++i) {
    for (unsigned i = 0; i < modelData.getFacets().size(); ++i) {
      glDrawElements(
        //   GL_LINE_LOOP, modelData.facets[i].numbers_of_vertexes_in_facets,
        GL_LINE_LOOP, modelData.getFacets()[i].size(),
        //   GL_UNSIGNED_INT, modelData.facets[i].vertexes);  // Draw lines
        GL_UNSIGNED_INT, modelData.getFacets()[i].data());  // Draw lines
    }

    glDisableClientState(GL_VERTEX_ARRAY);  // Take back permission

    shader->Unbind();  // Unbinding shader program so we could use OpenGL
                       // functions
  }
}

float lines[] = {-1000000, 0, 0, 1000000, 0,        0, 0, -1000000, 0, 0,
                 1000000,  0, 0, 0,       -1000000, 0, 0, 1000000};

float arrows[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

void GLWidget::drawCoordinates() {
  shader->Bind();
  refreshSettings(false);

  for (int i = 0; i < 3; ++i) {
    if (i == 0)
      shader->SetUniform4f("u_Color", 1, 0, 0, 1);
    else if (i == 1)
      shader->SetUniform4f("u_Color", 0, 1, 0, 1);
    else if (i == 2)
      shader->SetUniform4f("u_Color", 0, 0, 1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, lines);
    glDrawArrays(GL_LINES, i * 2, 2);
    glDisableClientState(GL_VERTEX_ARRAY);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, arrows);
    glDrawArrays(GL_POINTS, i, 1);
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  shader->Unbind();
}

void GLWidget::refreshSettings(bool needNormalize) {
  // Set transformation matrices

  matrix_t scaleWindow = setScale(width > height ? height / width : 1,
                                  height > width ? width / height : 1, 1);
  matrix_t rotation = setRotation(xRot, yRot, zRot);
  matrix_t trans = setTranslation(xTrans, yTrans, zTrans);
  matrix_t scale =
      needNormalize ? setScale(xScale, yScale, zScale) : setScale(1, 1, 1);
  matrix_t proj = perspective
                      ? setPerspective(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0)
                      : setOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);

  matrix_t normalized_rotate_pre_scale, start_rotate, normalized_rotate_scale,
      normalized_rotate_scale_trans, MVP;

  // Multiply matrices in the certain order

  needNormalize ? s21_mult_matrix(&matrixNormalized, &rotation, &start_rotate)
                : s21_mult_matrix(&matrixIdentity, &rotation, &start_rotate);
  s21_mult_matrix(&start_rotate, &scaleWindow, &normalized_rotate_pre_scale);
  s21_mult_matrix(&normalized_rotate_pre_scale, &scale,
                  &normalized_rotate_scale);
  s21_mult_matrix(&normalized_rotate_scale, &trans,
                  &normalized_rotate_scale_trans);
  s21_mult_matrix(&normalized_rotate_scale_trans, &proj, &MVP);

  // Set up shader uniforms and other preferences
  (pointSmooth && needNormalize) ? glEnable(GL_POINT_SMOOTH)
                                 : glDisable(GL_POINT_SMOOTH);
  (dashedLines && needNormalize) ? glEnable(GL_LINE_STIPPLE)
                                 : glDisable(GL_LINE_STIPPLE);
  needNormalize ? glLineWidth(lineWidth) : glLineWidth(1.0);
  shader->SetUniform4f("u_Color", mRed, mGreen, mBlue, 1.0);
  shader->SetUniform1f("pointSize", needNormalize ? pointSize : 20.0);
  shader->SetUniformMat4f("u_MVP", MVP.matrix[0]);

  // Deleting matrices
  s21_remove_matrix(&scaleWindow);
  s21_remove_matrix(&scale);
  s21_remove_matrix(&trans);
  s21_remove_matrix(&proj);
  s21_remove_matrix(&start_rotate);
  s21_remove_matrix(&normalized_rotate_pre_scale);
  s21_remove_matrix(&normalized_rotate_scale);
  s21_remove_matrix(&normalized_rotate_scale_trans);
  s21_remove_matrix(&MVP);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  mPos = event->pos();  // Remember position where cursor was clicked
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  // Calculate delta (mPos & currentPos) to set up Rot value or zTrans value
  if (ctrl) {
    zRot = -1 / M_PI * (event->pos().y() - mPos.y());
  } else {
    xRot = 1 / M_PI * (event->pos().y() - mPos.y());
    yRot = 1 / M_PI * (event->pos().x() - mPos.x());
  }
  update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  remember_rotation_matrix();  // Copy current matrix to previos matrix
}

void GLWidget::copyMatrix(matrix_t *from, matrix_t *to) {
  if (!check_matrices_can_sum(from, to)) {
    for (int i = 0; i < from->rows; ++i) {
      for (int j = 0; j < from->columns; ++j) {
        to->matrix[i][j] = from->matrix[i][j];
      }
    }
  }
}

matrix_t GLWidget::setTranslation(float d_x, float d_y, float d_z) {
  matrix_t M;
  s21_create_matrix(4, 4, &M);

  for (int i = 0; i < 4; ++i) {
    M.matrix[i][i] = 1.0;
  }

  M.matrix[3][0] = d_x;
  M.matrix[3][1] = d_y;
  M.matrix[3][2] = d_z;

  return M;
}

matrix_t GLWidget::setPerspective(float l, float r, float b, float t, float n,
                                  float f) {
  matrix_t M;
  s21_create_matrix(4, 4, &M);

  M.matrix[0][0] = n / r;
  M.matrix[0][1] = 0;
  M.matrix[0][2] = 0;
  M.matrix[0][3] = 0;
  M.matrix[1][0] = 0;
  M.matrix[1][1] = n / t;
  M.matrix[1][2] = 0;
  M.matrix[1][3] = 0;
  M.matrix[2][0] = 0;
  M.matrix[2][1] = 0;
  M.matrix[2][2] = -(f + n) / (f - n);
  M.matrix[2][3] = -1;
  M.matrix[3][0] = 0;
  M.matrix[3][1] = 0;
  M.matrix[3][2] = -2 * f * n / (f - n);
  M.matrix[3][3] = 0;

  return M;
}

matrix_t GLWidget::setOrtho(float l, float r, float b, float t, float n,
                            float f) {
  matrix_t M;
  s21_create_matrix(4, 4, &M);

  M.matrix[0][0] = 2 / (r - l);
  M.matrix[0][1] = 0;
  M.matrix[0][2] = 0;
  M.matrix[0][3] = 0;
  M.matrix[1][0] = 0;
  M.matrix[1][1] = 2 / (t - b);
  M.matrix[1][2] = 0;
  M.matrix[1][3] = 0;
  M.matrix[2][0] = 0;
  M.matrix[2][1] = 0;
  M.matrix[2][2] = -2 / (f - n);
  M.matrix[2][3] = 0;
  M.matrix[3][0] = -(r + l) / (r - l);
  M.matrix[3][1] = -(t + b) / (t - b);
  M.matrix[3][2] = -(f + n) / (f - n);
  M.matrix[3][3] = 1;

  return M;
}

matrix_t GLWidget::setIdentityMatrix() {
  matrix_t M = init_struct();
  s21_create_matrix(4, 4, &M);
  for (int i = 0; i < 4; ++i) {
    M.matrix[i][i] = 1;
  }
  return M;
}

matrix_t GLWidget::setRotation(float x, float y, float z) {
  matrix_t rotate_x = _setRotation(x, 1, 0, 0);
  matrix_t rotate_y = _setRotation(y, 0, 1, 0);
  matrix_t rotate_z = _setRotation(z, 0, 0, 1);

  matrix_t tmp_1, tmp_2, tmp_3;

  s21_mult_matrix(&matrixRotationPrevios, &rotate_x, &tmp_1);
  s21_mult_matrix(&tmp_1, &rotate_y, &tmp_2);
  s21_mult_matrix(&tmp_2, &rotate_z, &tmp_3);

  copyMatrix(&tmp_3, &matrixRotationCurrent);

  s21_remove_matrix(&rotate_x);
  s21_remove_matrix(&rotate_y);
  s21_remove_matrix(&rotate_z);
  s21_remove_matrix(&tmp_1);
  s21_remove_matrix(&tmp_2);
  s21_remove_matrix(&tmp_3);

  return matrixRotationCurrent;
}

matrix_t GLWidget::_setRotation(float angle, int x, int y, int z) {
  matrix_t M = init_struct();
  matrix_t MT = init_struct();

  if ((x + y + z) <= 1) {
    angle = angle * M_PI / 180.0f;
    s21_create_matrix(4, 4, &M);

    if (x) {
      M.matrix[0][0] = 1;
      M.matrix[1][1] = cos(angle);
      M.matrix[1][2] = -sin(angle);
      M.matrix[2][1] = sin(angle);
      M.matrix[2][2] = cos(angle);
      M.matrix[3][3] = 1;
    } else if (y) {
      M.matrix[0][0] = cos(angle);
      M.matrix[0][2] = sin(angle);
      M.matrix[1][1] = 1;
      M.matrix[2][0] = -sin(angle);
      M.matrix[2][2] = cos(angle);
      M.matrix[3][3] = 1;
    } else if (z) {
      M.matrix[0][0] = cos(angle);
      M.matrix[0][1] = -sin(angle);
      M.matrix[1][0] = sin(angle);
      M.matrix[1][1] = cos(angle);
      M.matrix[2][2] = 1;
      M.matrix[3][3] = 1;
    }

    s21_transpose(&M, &MT);
    s21_remove_matrix(&M);
  } else {
    qDebug() << "Wrong rotation input";
  }

  return MT;
}

matrix_t GLWidget::setScale(float x) { return setScale(x, x, x); }

matrix_t GLWidget::setScale(float x, float y, float z) {
  matrix_t M;
  s21_create_matrix(4, 4, &M);

  M.matrix[0][0] = x /** ((width > height) ? height/width : 1)*/;
  M.matrix[1][1] = y /** ((height > width) ? width/height : 1)*/;
  M.matrix[2][2] = z;
  M.matrix[3][3] = 1;

  return M;
}

void GLWidget::setNormalizedMatrix() {
  float xCenterTrans =
    //   (modelData.x_min + (modelData.x_max - modelData.x_min) / 2);
      (modelData.getMinX() + (modelData.getMaxX() - modelData.getMinX()) / 2);
  float yCenterTrans =
    //   (modelData.y_min + (modelData.y_max - modelData.y_min) / 2);
            (modelData.getMinY() + (modelData.getMaxY() - modelData.getMinY()) / 2);
  float zCenterTrans =
    //   (modelData.z_min + (modelData.z_max - modelData.z_min) / 2);
            (modelData.getMinZ() + (modelData.getMaxZ() - modelData.getMinZ()) / 2);

//   float CenterScaleX = 2 / (modelData.x_max - modelData.x_min);
//   float CenterScaleY = 2 / (modelData.y_max - modelData.y_min);
//   float CenterScaleZ = 2 / (modelData.z_max - modelData.z_min);


  float CenterScaleX = 2 / (modelData.getMaxX() - modelData.getMinX());
  float CenterScaleY = 2 / (modelData.getMaxY() - modelData.getMinY());
  float CenterScaleZ = 2 / (modelData.getMaxZ() - modelData.getMinZ());

  float CenterScale = CenterScaleX < CenterScaleY ? CenterScaleX : CenterScaleY;
  CenterScale = CenterScale < CenterScaleZ ? CenterScale : CenterScaleZ;

  matrix_t trans = setTranslation(-xCenterTrans, -yCenterTrans, -zCenterTrans);
  matrix_t scale = setScale(CenterScale);
  matrix_t tmp_1, tmp_2;

  s21_mult_matrix(&matrixIdentity, &trans, &tmp_1);
  s21_mult_matrix(&tmp_1, &scale, &tmp_2);

  copyMatrix(&tmp_2, &matrixNormalized);

  s21_remove_matrix(&trans);
  s21_remove_matrix(&scale);
  s21_remove_matrix(&tmp_1);
  s21_remove_matrix(&tmp_2);
}

void GLWidget::remember_rotation_matrix() {
  xRot = yRot = zRot = 0;
  copyMatrix(&matrixRotationCurrent, &matrixRotationPrevios);
}

void GLWidget::initRotationMatrix() {
  // Set RotationMatrices equal to IdenticalMatrix; Needs to set rotation to 0
  copyMatrix(&matrixIdentity, &matrixRotationCurrent);
  remember_rotation_matrix();
}

void GLWidget::setColor(bool background, float r, float g, float b) {
  if (background) {
    bRed = r;
    bGreen = g;
    bBlue = b;
  } else {
    mRed = r;
    mGreen = g;
    mBlue = b;
  }
}
