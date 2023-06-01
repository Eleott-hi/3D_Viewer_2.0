#include "ITechnique.h"

namespace s21

{

QMatrix4x4 ITechnique::projection_, ITechnique::view_, ITechnique::model_;

void ITechnique::SetModelMatrix(QMatrix4x4 model) {
  model_ = model;
  OnMVPLoaded();
}

void ITechnique::SetProjectionViewMatrix(QMatrix4x4 proj, QMatrix4x4 view) {
  projection_ = proj, view_ = view;
}

void ITechnique::GenerateShaders(QString vertex_file,    //
                                 QString geometry_file,  //
                                 QString fragment_file) {
  shader_.GenerateShaders(vertex_file, geometry_file, fragment_file);
}

}  // namespace s21
