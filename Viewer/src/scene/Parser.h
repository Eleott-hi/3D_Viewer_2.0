#pragma once

#include "core/ECS_Controller.h"

namespace s21 {

class Parser {
 public:
  Parser() = default;
  ~Parser() = default;

  void loadModel(ECS_Controller *scene, std::string const &filename);

  //  private:
  // std::vector<ParsingData> data_;
  // std::string directory_;

  // void processNode(aiNode *node, const aiScene *scene);
  // Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  // QVector<Vertex> loadVertices(aiMesh *mesh, const aiScene *scene);
  // Material LoadMaterial(aiMesh *mesh, const aiScene *scene);
  // QVector<quint32> LoadIndices(aiMesh *mesh, const aiScene *scene);
  // void LoadTexture(aiMaterial *material, aiTextureType type, Texture
  // &texture);
};

}  // namespace s21
