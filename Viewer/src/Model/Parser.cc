#include "Parser.h"

#include "Utils.h"

namespace s21 {

// =============================== UTILS ===============================

void Parser::findMinMax(float x, float y, float z) {
  minX_ = std::min(minX_, x);
  minY_ = std::min(minY_, y);
  minZ_ = std::min(minZ_, z);
  maxX_ = std::max(maxX_, x);
  maxY_ = std::max(maxY_, y);
  maxZ_ = std::max(maxZ_, z);
}
// =============================== UTILS ===============================

std::optional<Model> Parser::loadModel(QString filename) {
  for (auto const &model : loadedMeshes_)
    if (model.filename == filename) return model;

  if (ParseModelFile(filename)) return loadedMeshes_.back();

  return {};
}

bool Parser::ParseModelFile(QString filename) {
  Assimp::Importer importer;

  uint flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
               aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

  const aiScene *scene = importer.ReadFile(filename.toStdString(), flags);

  if (!scene ||                                     //
      scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||  //
      !scene->mRootNode) {
    qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString();
    return false;
  }

  loadedMeshes_.push_back({filename, QVector<Mesh>{}});

  minX_ = minY_ = minZ_ = std::numeric_limits<float>::infinity();
  maxX_ = maxY_ = maxZ_ = -std::numeric_limits<float>::infinity();
  processNode(scene->mRootNode, scene);

  // Utils::setNormalizeMatrix(mSettings_, {minX_, minY_, minZ_},
  //                           {maxX_, maxY_, maxZ_});
  return true;
}

void Parser::processNode(aiNode *node, const aiScene *scene) {
  auto &model = loadedMeshes_.back();

  for (quint32 i = 0; i < node->mNumMeshes; i++)
    model.meshes << processMesh(scene->mMeshes[node->mMeshes[i]], scene);

  for (quint32 i = 0; i < node->mNumChildren; i++)
    processNode(node->mChildren[i], scene);
}

Mesh Parser::processMesh(aiMesh *mesh, const aiScene *scene) {
  return {0, loadVertices(mesh, scene), LoadIndices(mesh, scene)};
}

QVector<Vertex> Parser::loadVertices(aiMesh *mesh, const aiScene *scene) {
  QVector<Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);

  for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    float x = mesh->mVertices[i].x;
    float y = mesh->mVertices[i].y;
    float z = mesh->mVertices[i].z;

    findMinMax(x, y, z);
    vertex.position = {x, y, z};

    if (mesh->HasNormals())
      vertex.normal = {mesh->mNormals[i].x,  //
                       mesh->mNormals[i].y,  //
                       mesh->mNormals[i].z};

    if (mesh->mTextureCoords[0])
      vertex.tex_coords = {mesh->mTextureCoords[0][i].x,
                           mesh->mTextureCoords[0][i].y};

    vertices << vertex;
  }

  return vertices;
}
QVector<quint32> Parser::LoadIndices(aiMesh *mesh, const aiScene *scene) {
  QVector<quint32> indices;

  for (quint32 i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    for (quint32 j = 0; j < face.mNumIndices; j++) indices << face.mIndices[j];
  }

  return indices;
}

}  // namespace s21
