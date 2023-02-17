#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <string>
#include <vector>

#include "components/Components.h"
#include "TextureStorage.h"

namespace s21 {
class ModelParser {
 public:
  ModelParser(std::shared_ptr<TextureStorage> const &storage)
      : textureStorage(storage){};
  ~ModelParser() = default;
  std::pair<bool, MeshComponent> loadModel(std::string const &filename);

 private:
  std::shared_ptr<TextureStorage> textureStorage;
  std::vector<MeshComponent> loadedMeshes;
  float minX_, minY_, minZ_, maxX_, maxY_, maxZ_;

  bool ParseModelFile(std::string const &filename);
  void processNode(aiNode *node, const aiScene *scene);
  s_Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Vertex> loadVertices(aiMesh *mesh, const aiScene *scene);
  std::vector<uint32_t> LoadIndices(aiMesh *mesh, const aiScene *scene);

  // ==================== utils =====================
  void findMinMax(float x, float y, float z);
};
}  // namespace s21
