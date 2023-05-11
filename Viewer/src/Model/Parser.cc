#include "Parser.h"

#include "Utils.h"

namespace s21 {

ParsingData Parser::loadModel(std::string const &filename) {
  Assimp::Importer importer;

  uint32_t flags = aiProcess_Triangulate |       //
                   aiProcess_GenSmoothNormals |  //
                   aiProcess_FlipUVs |           //
                   aiProcess_CalcTangentSpace;
  const aiScene *scene = importer.ReadFile(filename, flags);

  if (!scene ||                                     //
      scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||  //
      !scene->mRootNode) {
    qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString();
    return {};
  }

  directory_ = filename.substr(0, filename.find_last_of('/'));

  data_.model = {filename, QVector<Mesh>{}};
  processNode(scene->mRootNode, scene);
  return data_;
}

void Parser::processNode(aiNode *node, const aiScene *scene) {
  for (quint32 i = 0; i < node->mNumMeshes; i++)
    data_.model->meshes << processMesh(scene->mMeshes[node->mMeshes[i]], scene);

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

    if (mesh->HasNormals()) {
      auto const &[x, y, z] = mesh->mNormals[i];
      vertex.normal = {x, y, z};
    }

    if (mesh->mTextureCoords[0]) {
      {
        auto const &[x, y, z] = mesh->mTextureCoords[0][i];
        vertex.tex_coords = {x, y};
      }
      {
        auto const &[x, y, z] = mesh->mTangents[i];
        vertex.tangent = {x, y, z};
      }
      {
        auto const &[x, y, z] = mesh->mBitangents[i];
        vertex.bitangent = {x, y, z};
      }
    }

    auto const &[x, y, z] = mesh->mVertices[i];
    vertex.position = {x, y, z};

    vertices << vertex;
  }

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  LoadTexture(material, aiTextureType_DIFFUSE, data_.diffuseMap);
  LoadTexture(material, aiTextureType_HEIGHT, data_.normalMap);
  LoadTexture(material, aiTextureType_SPECULAR, data_.specularMap);

  // aiColor3D color(0.0f, 0.0f, 0.0f);
  // float d = 0;
  // material->Get(AI_MATKEY_COLOR_AMBIENT, color);

  // qDebug() << color.r << " " << color.g << " " << color.b;

  // material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
  // qDebug() << color.r << " " << color.g << " " << color.b;

  // material->Get(AI_MATKEY_COLOR_SPECULAR, color);
  // qDebug() << color.r << " " << color.g << " " << color.b;

  // material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
  // qDebug() << color.r << " " << color.g << " " << color.b;

  // material->Get(AI_MATKEY_SHININESS, d);
  // qDebug() << d;

  // material->Get(AI_MATKEY_OPACITY, d);
  // qDebug() << d;
  return vertices;
}

void Parser::LoadTexture(aiMaterial *material, aiTextureType type,
                         std::optional<Texture> &texture) {
  for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {
    aiString str;
    material->GetTexture(type, i, &str);

    std::string const &filename = directory_ + "/" + str.C_Str();
    texture = {textureStorage_->LoadTexture(filename), "specularMap"};
  }
}

QVector<quint32> Parser::LoadIndices(aiMesh *mesh, const aiScene *scene) {
  QVector<quint32> indices;

  for (quint32 i = 0; i < mesh->mNumFaces; i++)
    indices << QVector<quint32>(
        mesh->mFaces[i].mIndices,
        mesh->mFaces[i].mIndices + mesh->mFaces[i].mNumIndices);

  return indices;
}

}  // namespace s21
