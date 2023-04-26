#include "RenderSystem.h"

namespace s21 {

void RenderSystem::Init(std::shared_ptr<ECS_Controller> const &scene,
                        std::shared_ptr<TechniqueStrategy> const &technique) {
  scene_ = scene;
  technique_ = technique;
}

void RenderSystem::Update(EntityID camera) {
  UpdateLightData();

  auto const &[proj, view] = scene_->GetComponent<CameraComponent>(camera);

  for (auto &entity : entities_) {
    auto &model = scene_->GetComponent<MeshComponent>(entity);
    auto const &transform = scene_->GetComponent<TransformComponent>(entity);
    auto const &lineSettings =
        scene_->GetComponent<LineSettingsComponent>(entity);
    auto const &pointSettings =
        scene_->GetComponent<PointSettingsComponent>(entity);
    auto const &[shininess] = scene_->GetComponent<MaterialComponent>(entity);

    UpdateGeometrySettings(pointSettings, lineSettings);

    QMatrix4x4 modelMatrix = transform.GetModelMatrix();

    // ============================ Draw Points ============================
    if (pointSettings.show) {
      technique_->Enable(TechniqueType::SIMPLE_COLOR);
      technique_->setMVP(proj, view, modelMatrix);
      technique_->setColor(pointSettings.color);

      DrawObject(model, GL_POINTS);
    }

    // ============================ Draw Object ============================
    auto &[tecniqueStrategy] = scene_->GetComponent<TechniqueComponent>(entity);
    auto &texture = scene_->GetComponent<TextureComponent>(entity);

    technique_->Enable(tecniqueStrategy);
    technique_->setMVP(proj, view, modelMatrix);

    switch (tecniqueStrategy) {
      case TechniqueType::SIMPLE_COLOR:
        technique_->setColor(lineSettings.color);
        break;

      case TechniqueType::SIMPLE_TEXTURE:
        technique_->setTexture(texture);
        break;

      case TechniqueType::LIGHT_COLOR:
        technique_->setColor(lineSettings.color);
        technique_->setShininess(shininess);
        break;

      case TechniqueType::LIGHT_TEXTURE:
        technique_->setTexture(texture);
        technique_->setShininess(shininess);
        break;

      default:
        assert(false && "RenderSystem::Update: no such tecnique strategy");
        break;
    }

    DrawObject(model);

    NormalizeOpenGLSettings();
  }
}

void RenderSystem::UpdateLightData() {
  auto pointLights = scene_->GetEntities<PointLightComponent>();

  std::vector<TransformComponent> transforms;
  std::vector<LightSettingsComponent> settings;

  transforms.reserve(pointLights.size());
  settings.reserve(pointLights.size());

  for (auto const &entity : pointLights) {
    auto const &transform = scene_->GetComponent<TransformComponent>(entity);
    auto const &setting = scene_->GetComponent<LightSettingsComponent>(entity);
    transforms.push_back(transform);
    settings.push_back(setting);
  }

  if (pointLights.size() == 0) {
    transforms.push_back({});
    settings.push_back({});
  }

  technique_->Enable(TechniqueType::LIGHT_COLOR);
  technique_->ApplyLightSettings(transforms, settings);

  technique_->Enable(TechniqueType::LIGHT_TEXTURE);
  technique_->ApplyLightSettings(transforms, settings);
}

void RenderSystem::UpdateGeometrySettings(
    PointSettingsComponent const &pointSettings,
    LineSettingsComponent const &lineSettings) {
  auto const &[pointShow, pointSmooth, pointSize, pointColor] = pointSettings;
  auto const &[lineShow, lineDashed, lineSize, lineColor] = lineSettings;

  pointSmooth ? glEnable(GL_POINT_SMOOTH) : glDisable(GL_POINT_SMOOTH);
  lineDashed ? glEnable(GL_LINE_STIPPLE) : glDisable(GL_LINE_STIPPLE);
  lineShow ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
           : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glLineWidth(lineSize);
  glPointSize(pointSize);
}

void RenderSystem::NormalizeOpenGLSettings() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_LINE_STIPPLE);
  glDisable(GL_POINT_SMOOTH);
  glPointSize(1);
  glLineWidth(1);
}

void RenderSystem::DrawObject(MeshComponent &model, GLenum form) {
  for (auto &mesh : model.meshes_) {
    if (!mesh.VAO) bufferize(mesh);

    glBindVertexArray(mesh.VAO);
    glDrawElements(form, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

void RenderSystem::bufferize(s_Mesh &mesh) {
  auto &[VAO, vertices, indices] = mesh;
  uint32_t VBO = 0, EBO = 0;

  qDebug()<<"Bufferize: "<<VAO<<" "<<vertices.size()<<" "<<indices.size();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  // vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Position));
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));
  glBindVertexArray(0);
}

}  // namespace s21
