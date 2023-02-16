// #define STB_IMAGE_IMPLEMENTATION

#include "model.h"

#include "stb_image.h"

namespace s21 {

// =============================== UTILS ===============================
namespace Utils {
static void setNormalizeMatrix(ModelSettings &settings, const QVector3D &min,
                               const QVector3D &max) {
  QVector3D CenterTranslate = {(min.x() + max.x()) / 2.0f,
                               (min.y() + max.y()) / 2.0f,
                               (min.z() + max.z()) / 2.0f};
  std::vector<float> scales = {2.0f / (max.x() - min.x()),
                               2.0f / (max.y() - min.y()),
                               2.0f / (max.z() - min.z())};
  QMatrix4x4 m;

  m.scale(*(std::min_element(scales.begin(), scales.end())));
  m.translate(-CenterTranslate);

  settings.setNormalizeMatrix(m);
}

template <typename Type>
static void append(std::vector<Type> &to, std::vector<Type> &from) {
  to.insert(to.end(), from.begin(), from.end());
}

}  // namespace Utils

void Model::findMinMax(float x, float y, float z) {
  minX_ = std::min(minX_, x);
  minY_ = std::min(minY_, y);
  minZ_ = std::min(minZ_, z);
  maxX_ = std::max(maxX_, x);
  maxY_ = std::max(maxY_, y);
  maxZ_ = std::max(maxZ_, z);
}
// =============================== UTILS ===============================

Model::Model(const std::string &path, TextureStorage *storage, bool gamma)
    : textureStorage_(storage), gammaCorrection_(gamma) {
  minX_ = minY_ = minZ_ = std::numeric_limits<float>::infinity();
  maxX_ = maxY_ = maxZ_ = -std::numeric_limits<float>::infinity();
  initializeOpenGLFunctions();
  loadModel(path);
}

// ============================== LOAD MODEL ==============================
void Model::loadModel(const std::string &filename) {
  Assimp::Importer importer;

  uint flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
               aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

  const aiScene *scene = importer.ReadFile(filename, flags);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString();
    return;
  }

  mSettings_.SetFilename(filename.c_str());

  processNode(scene->mRootNode, scene);

  Utils::setNormalizeMatrix(mSettings_, {minX_, minY_, minZ_},
                            {maxX_, maxY_, maxZ_});
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (uint32_t i = 0; i < node->mNumMeshes; i++)
    meshes_.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));

  for (uint32_t i = 0; i < node->mNumChildren; i++)
    processNode(node->mChildren[i], scene);
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  return std::make_shared<Mesh>(loadVertices(mesh, scene),
                                LoadIndices(mesh, scene),
                                loadTextures(mesh, scene));
}

// =============================== Vertex ===============================
std::vector<Vertex> Model::loadVertices(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;

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

  mSettings_.AddVerticies(vertices.size());

  return vertices;
}

// =============================== Index =================================
std::vector<uint32_t> Model::LoadIndices(aiMesh *mesh, const aiScene *scene) {
  std::vector<uint32_t> indices;

  for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (uint32_t j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  mSettings_.AddIndices(indices.size());

  return indices;
}

// =============================== Texture =========================+======
std::vector<TextureComponent> Model::loadTextures(aiMesh *mesh,
                                                  const aiScene *scene) {
  std::vector<TextureComponent> textures;
  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  std::vector<TextureComponent> diffuseMaps = loadMaterialTexture(
      material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
  std::vector<TextureComponent> specularMaps = loadMaterialTexture(
      material, aiTextureType_SPECULAR, TextureType::SPECULAR);
  std::vector<TextureComponent> normalMaps =
      loadMaterialTexture(material, aiTextureType_HEIGHT, TextureType::NORMAL);
  std::vector<TextureComponent> heightMaps =
      loadMaterialTexture(material, aiTextureType_AMBIENT, TextureType::HEIGHT);

  Utils::append(textures, diffuseMaps);
  Utils::append(textures, specularMaps);
  Utils::append(textures, normalMaps);
  Utils::append(textures, heightMaps);

  return textures;
}

std::vector<TextureComponent> Model::loadMaterialTexture(aiMaterial *mat,
                                                         aiTextureType type,
                                                         TextureType typeName) {
  std::vector<TextureComponent> textures;

  for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    std::string texPath = str.C_Str();

    if (iterator != textures_loaded_.end()) {
      textures.push_back(*iterator);
      return;
    }

    std::string filename = mSettings_.GetFilename().toStdString();
    std::string directory = filename.substr(0, filename.find_last_of('/'));
    uint32_t texture = TextureFromFile(directory + "/" + texPath);
    if (texture) {
      textures.push_back({texture, typeName});
      textures_loaded_.insert({texPath, {texture, typeName}});
    }
  }

  return textures;
}

// TODO: texture class
uint32_t Model::TextureFromFile(const std::string &filename, bool gamma) {
  uint32_t textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

  if (!data) {
    qDebug() << "Texture failed to load at path: " << filename.c_str();
    return textureID;
  }

  GLenum format;
  if (nrComponents == 1)
    format = GL_RED;
  else if (nrComponents == 3)
    format = GL_RGB;
  else if (nrComponents == 4)
    format = GL_RGBA;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  return textureID;
}

}  // namespace s21
