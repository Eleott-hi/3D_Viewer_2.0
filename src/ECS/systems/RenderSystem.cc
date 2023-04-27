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
    auto &model = scene_->GetComponent<ModelComponent>(entity);
    auto const &transform = scene_->GetComponent<TransformComponent>(entity);
    auto const &lineSettings =
        scene_->GetComponent<LineSettingsComponent>(entity);
    auto const &pointSettings =
        scene_->GetComponent<PointSettingsComponent>(entity);
    auto const &material = scene_->GetComponent<MaterialComponent>(entity);

    UpdateGeometrySettings(pointSettings, lineSettings);

    QMatrix4x4 modelMatrix = transform.GetModelMatrix();

    if (pointSettings.show) {
      technique_->Enable(TechniqueType::SIMPLE_COLOR);
      technique_->setMVP(proj, view, modelMatrix);
      technique_->setColor(pointSettings.color);

      DrawObject(model, GL_POINTS);
    }

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
        technique_->setMaterial(material);
        break;

      case TechniqueType::LIGHT_TEXTURE:
        technique_->setTexture(texture);
        technique_->setMaterial(material);
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
  static std::vector<TransformComponent> transforms;
  static std::vector<LightComponent> lights;

  auto pointLights = scene_->GetEntities<PointLightTag>();

  transforms.clear();
  lights.clear();

  transforms.reserve(pointLights.size());
  lights.reserve(pointLights.size());

  for (auto const &entity : pointLights) {
    auto const &transform = scene_->GetComponent<TransformComponent>(entity);
    auto const &light = scene_->GetComponent<LightComponent>(entity);
    transforms.push_back(transform);
    lights.push_back(light);
  }

  if (pointLights.size() == 0) transforms.push_back({}), lights.push_back({});

  technique_->Enable(TechniqueType::LIGHT_COLOR);
  technique_->ApplyLightSettings(transforms, lights);

  technique_->Enable(TechniqueType::LIGHT_TEXTURE);
  technique_->ApplyLightSettings(transforms, lights);
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

void RenderSystem::DrawObject(ModelComponent &model, GLenum form) {
  for (auto &mesh : model.meshes_) {
    if (!mesh.VAO) mesh.bufferize(this);

    glBindVertexArray(mesh.VAO);
    glDrawElements(form, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}

}  // namespace s21
