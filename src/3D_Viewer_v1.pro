QT       += core gui opengl openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(3rdparty/src/gifimage/qtgifimage.pri)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += GL_SILENCE_DEPRECATION
win32: LIBS += -lopengl32

SOURCES += \
    file_parcer.c \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    matrix_algebra/common.c \
    matrix_algebra/s21_calc_complements.c \
    matrix_algebra/s21_create_matrix.c \
    matrix_algebra/s21_determinant.c \
    matrix_algebra/s21_eq_matrix.c \
    matrix_algebra/s21_inverse_matrix.c \
    matrix_algebra/s21_mult_matrix.c \
    matrix_algebra/s21_mult_number.c \
    matrix_algebra/s21_remove_matrix.c \
    matrix_algebra/s21_sub_matrix.c \
    matrix_algebra/s21_sum_matrix.c \
    matrix_algebra/s21_transpose.c \
    shader.cpp

HEADERS += \
    file_parcer.h \
    glwidget.h \
    mainwindow.h \
    matrix_algebra/s21_matrix.h \
    shader.h 

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shaders.txt

