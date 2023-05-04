#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <QString>
#include <assimp/Importer.hpp>
#include <map>
#include <optional>
#include <vector>

// #include "TextureStorage.h"
#include "Components.h"

namespace s21 {
class Parser {
 public:
  // Parser(std::shared_ptr<TextureStorage> const &storage)
  // : textureStorage(storage){};
  ~Parser() = default;
  std::optional<Model> loadModel(QString filename);

 private:
  // std::shared_ptr<TextureStorage> textureStorage;
  QVector<Model> loadedMeshes_;
  float minX_, minY_, minZ_, maxX_, maxY_, maxZ_;

  bool ParseModelFile(QString filename);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  QVector<Vertex> loadVertices(aiMesh *mesh, const aiScene *scene);
  QVector<quint32> LoadIndices(aiMesh *mesh, const aiScene *scene);

  // ==================== utils =====================
  void findMinMax(float x, float y, float z);
};

}  // namespace s21
