#include "Parser.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <QString>
#include <algorithm>
#include <assimp/Importer.hpp>
#include <map>
#include <numeric>
#include <optional>
#include <vector>

#include "Components.h"
#include "Hierarchy.h"
#include "TextureStorage.h"
#include "Utils.h"

namespace s21 {

std::string
    dir =  //
           // "/opt/goinfre/pintoved/3D_Viewer_2.0/Tutorials/resources/";
    "C:/Users/lapte/Desktop/Portfolio/3D_Viewer_2.0/Tutorials/resources/";

std::string directory_;
QVector<EntityID> meshes;

QVector3D translation_ = {0.0f, 0.0f, 0.0f};

void LoadTexture(aiMaterial *material, aiTextureType type, Texture &texture) {
  for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {
    aiString str;
    material->GetTexture(type, i, &str);
    texture = TextureStorage::LoadTexture(directory_ + "/" + str.C_Str());
  }
}

QVector<quint32> LoadIndices(aiMesh *mesh, const aiScene *scene) {
  QVector<quint32> indices;

  for (quint32 i = 0; i < mesh->mNumFaces; i++)
    indices << QVector<quint32>(
        mesh->mFaces[i].mIndices,
        mesh->mFaces[i].mIndices + mesh->mFaces[i].mNumIndices);

  return indices;
}

Material LoadMaterial(aiMesh *mesh, const aiScene *scene) {
  Material material;
  aiMaterial *ai_material = scene->mMaterials[mesh->mMaterialIndex];

  LoadTexture(ai_material, aiTextureType_HEIGHT, material.normal);
  LoadTexture(ai_material, aiTextureType_DIFFUSE, material.diffuse);
  LoadTexture(ai_material, aiTextureType_SPECULAR, material.specular);
  material.ao = TextureStorage::LoadTexture(dir + "objects/backpack/ao.jpg");
  material.roughness =
      TextureStorage::LoadTexture(dir + "objects/backpack/roughness.jpg");

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

  return material;
}

QVector<Vertex> loadVertices(aiMesh *mesh, const aiScene *scene) {
  QVector<Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);

  aiVector3D minBound(std::numeric_limits<float>::infinity());
  aiVector3D maxBound(-std::numeric_limits<float>::infinity());

  for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
    auto const &[px, py, pz] = mesh->mVertices[i];

    minBound.x = std::min(minBound.x, px);
    minBound.y = std::min(minBound.y, py);
    minBound.z = std::min(minBound.z, pz);
    maxBound.x = std::max(maxBound.x, px);
    maxBound.y = std::max(maxBound.y, py);
    maxBound.z = std::max(maxBound.z, pz);
  }

  aiVector3D center = (minBound + maxBound) * 0.5f;
  translation_ = {center.x, center.y, center.z};

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    mesh->mVertices[i] -= center;

    Vertex vertex;

    if (mesh->HasNormals()) {
      auto const &[nx, ny, nz] = mesh->mNormals[i];
      vertex.normal = {nx, ny, nz};
    }

    if (mesh->mTextureCoords[0]) {
      auto const &[cx, cy, _] = mesh->mTextureCoords[0][i];
      vertex.tex_coords = {cx, cy};
      auto const &[tx, ty, tz] = mesh->mTangents[i];
      vertex.tangent = {tx, ty, tz};
      auto const &[bx, by, bz] = mesh->mBitangents[i];
      vertex.bitangent = {bx, by, bz};
    }

    auto const &[px, py, pz] = mesh->mVertices[i];
    vertex.position = {px, py, pz};

    vertices << std::move(vertex);
  }

  return vertices;
}

void processMesh(ECS_Controller *ecs_scene, aiMesh *ai_mesh,
                 const aiScene *scene) {
  Mesh mesh = {loadVertices(ai_mesh, scene), LoadIndices(ai_mesh, scene)};
  Material material = LoadMaterial(ai_mesh, scene);

  EntityID entity = ecs_scene->NewEntity();
  ecs_scene->AddComponent<Shader>(entity,
                                  {TechniqueType::PHYSICAL_BASED_RENDERING});
  ecs_scene->AddComponent<Transform>(entity, {translation_});
  ecs_scene->AddComponent<RenderTag>(entity);
  ecs_scene->AddComponent<ShadowTag>(entity);
  ecs_scene->AddComponent<Material>(entity, material);
  ecs_scene->AddComponent<Mesh>(entity, std::move(mesh));

  meshes.push_back(entity);
}

void processNode(ECS_Controller *ecs_scene, aiNode *node,
                 const aiScene *ai_scene) {
  for (quint32 i = 0; i < node->mNumMeshes; i++)
    processMesh(ecs_scene, ai_scene->mMeshes[node->mMeshes[i]], ai_scene);

  for (quint32 i = 0; i < node->mNumChildren; i++)
    processNode(ecs_scene, node->mChildren[i], ai_scene);
}

void Parser::loadModel(ECS_Controller *ecs_scene, std::string const &filename) {
  Assimp::Importer importer;
  uint32_t flags = aiProcess_Triangulate |       //
                   aiProcess_GenSmoothNormals |  //
                   aiProcess_FlipUVs |           //
                   aiProcess_CalcTangentSpace;
  const aiScene *scene = importer.ReadFile(filename, flags);

  meshes.clear();

  if (!scene ||                                     //
      scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||  //
      !scene->mRootNode) {
    qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString();
    return;
  }

  directory_ = filename.substr(0, filename.find_last_of('/'));

  processNode(ecs_scene, scene->mRootNode, scene);

  if (meshes.empty()) return;

  Transform t;
  t.rotation = {0, 0, 0, 1};

  EntityID entity = ecs_scene->NewEntity();
  ecs_scene->AddComponent<Transform>(entity, t);
  ecs_scene->AddComponent<HierarchyComponent>(entity);
  // ecs_scene->AddComponent<Model>(entity, {filename});

  Hierarchy::AddChildren(ecs_scene, entity, meshes);
}

}  // namespace s21
