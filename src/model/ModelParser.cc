#include "ModelParser.h"

#include "Utils.h"

namespace s21 {

// =============================== UTILS ===============================

void ModelParser::findMinMax(float x, float y, float z) {
  minX_ = std::min(minX_, x);
  minY_ = std::min(minY_, y);
  minZ_ = std::min(minZ_, z);
  maxX_ = std::max(maxX_, x);
  maxY_ = std::max(maxY_, y);
  maxZ_ = std::max(maxZ_, z);
}
// =============================== UTILS ===============================

std::pair<bool, ModelComponent> ModelParser::loadModel(
    std::string const &filename) {
  for (auto model : loadedMeshes)
    if (model.filename_ == filename) return {true, model};

  bool result = ParseModelFile(filename);

  return {result, loadedMeshes.back()};
}

bool ModelParser::ParseModelFile(std::string const &filename) {
  Assimp::Importer importer;

  uint flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
               aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

  const aiScene *scene = importer.ReadFile(filename, flags);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString();
    return false;
  }

  loadedMeshes.push_back({filename, std::vector<Mesh>{}});

  minX_ = minY_ = minZ_ = std::numeric_limits<float>::infinity();
  maxX_ = maxY_ = maxZ_ = -std::numeric_limits<float>::infinity();
  processNode(scene->mRootNode, scene);

  // Utils::setNormalizeMatrix(mSettings_, {minX_, minY_, minZ_},
  //                           {maxX_, maxY_, maxZ_});
  return true;
}

void ModelParser::processNode(aiNode *node, const aiScene *scene) {
  auto &model = loadedMeshes.back();

  for (uint32_t i = 0; i < node->mNumMeshes; i++)
    model.meshes_.push_back(
        processMesh(scene->mMeshes[node->mMeshes[i]], scene));

  for (uint32_t i = 0; i < node->mNumChildren; i++)
    processNode(node->mChildren[i], scene);
}

Mesh ModelParser::processMesh(aiMesh *mesh, const aiScene *scene) {
  return {0, loadVertices(mesh, scene), LoadIndices(mesh, scene)};
}

// =============================== Vertex ===============================
std::vector<Vertex> ModelParser::loadVertices(aiMesh *mesh,
                                              const aiScene *scene) {
  std::vector<Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);

  for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    float x = mesh->mVertices[i].x,  //
        y = mesh->mVertices[i].y,    //
        z = mesh->mVertices[i].z;
    findMinMax(x, y, z);
    vertex.Position = {x, y, z};

    if (mesh->HasNormals())
      vertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                       mesh->mNormals[i].z};

    if (mesh->mTextureCoords[0])
      vertex.TexCoords = {mesh->mTextureCoords[0][i].x,
                          mesh->mTextureCoords[0][i].y};

    vertices.push_back(vertex);
  }

  return vertices;
}

// =============================== Index =================================
std::vector<uint32_t> ModelParser::LoadIndices(aiMesh *mesh,
                                               const aiScene *scene) {
  std::vector<uint32_t> indices;

  for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (uint32_t j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  return indices;
}

}  // namespace s21
