#ifndef SRC_HEADERS_MODEL_H
#define SRC_HEADERS_MODEL_H

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <QOpenGLExtraFunctions>
#include <assimp/Importer.hpp>
#include <list>
#include <map>
#include <string>

#include "mesh.h"
#include "model_settings.h"
namespace s21 {

class Model : protected QOpenGLExtraFunctions {
 public:
  Model() = delete;
  ~Model() = default;
  Model(Model &&m) = default;
  Model(const Model &m) = default;
  Model &operator=(Model &&m) = default;
  Model &operator=(const Model &m) = default;
  Model(const std::string &path, TextureStorage *storage, bool gamma = false);

  std::vector<std::shared_ptr<Mesh>> GetMeshes() {  //
    return meshes_;
  }

  auto &GetTextures() {  //
    return textures_loaded_;
  }

  ModelSettings &getSettings() {  //
    return mSettings_;
  }

  void Draw() {
    for (auto &mesh : meshes_) mesh->Draw();
  }

 private:
  void loadModel(const std::string &path);
  void findMinMax(float x, float y, float z);
  void processNode(aiNode *node, const aiScene *scene);
  std::vector<Vertex> loadVertices(aiMesh *mesh, const aiScene *scene);
  std::vector<TextureComponent> loadTextures(aiMesh *mesh,
                                             const aiScene *scene);
  std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
  unsigned int TextureFromFile(const std::string &filename, bool gamma = false);
  std::vector<unsigned int> LoadIndices(aiMesh *mesh, const aiScene *scene);
  std::vector<TextureComponent> loadMaterialTexture(aiMaterial *mat,
                                                    aiTextureType type,
                                                    TextureType typeName);

 protected:
  ModelSettings mSettings_;

 private:
  TextureStorage *textureStorage_;

  bool gammaCorrection_;
  std::string directory_;
  std::vector<std::shared_ptr<Mesh>> meshes_;
  float minX_, minY_, minZ_, maxX_, maxY_, maxZ_;
  std::map<std::string, TextureComponent> textures_loaded_;
};

}  // namespace s21

#endif  // SRC_HEADERS_MODEL_H
