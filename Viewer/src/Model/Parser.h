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
#include "TextureStorage.h"

namespace s21 {

struct ParsingData {
  std::optional<Model> model = std::nullopt;
  std::optional<Texture> normalMap = std::nullopt;
};

class Parser {
 public:
  Parser(TextureStorage *textureStorage) : textureStorage_(textureStorage) {}
  ~Parser() = default;
  ParsingData loadModel(QString filename);

 private:
  ParsingData data_;
  TextureStorage *textureStorage_ = nullptr;

  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  QVector<Vertex> loadVertices(aiMesh *mesh, const aiScene *scene);
  QVector<quint32> LoadIndices(aiMesh *mesh, const aiScene *scene);
};

}  // namespace s21
