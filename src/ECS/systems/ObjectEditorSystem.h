
#pragma once

#include <QColor>
#include <QOpenGLExtraFunctions>
#include <functional>

#include "ECS_Controller.h"
#include "signal_handler.h"

namespace s21 {

class ObjectEditorSystem : public System, protected QOpenGLExtraFunctions {
public:
  ~ObjectEditorSystem() = default;
  ObjectEditorSystem() { //
    initializeOpenGLFunctions();
  }

  void Init(std::shared_ptr<Controller> const &scene) { //
    scene_ = scene;
  }

  void ObjectSelected() {
    for (auto &entity : entities_) {
      //  Set Ui Info Panel
      {
        auto &[filename, meshes] = scene_->GetComponent<MeshComponent>(entity);
        uint32_t vertices = 0, indices = 0;
        for (auto &mesh : meshes) {
          vertices += mesh.vertices_.size(), indices += mesh.indices_.size();
        }
        emit signal_handler_.SetInfoPanel(filename.c_str(), vertices, indices);
      }

      //  Set Ui Transform Panel
      {
        auto &transform = scene_->GetComponent<TransformComponent>(entity);
        emit signal_handler_.SetTransformPanel(
            transform.position, transform.rotation, transform.scale);
      }

      //  Set Ui Point Panel
      {
        auto &[show, smooth, size, color] =
            scene_->GetComponent<PointSettingsComponent>(entity);
        emit signal_handler_.SetPointPanel(show, smooth, size, color);
      }

      //  Set Ui Line Panel
      {
        auto &[show, dashed, size, color] =
            scene_->GetComponent<LineSettingsComponent>(entity);
        emit signal_handler_.SetLinePanel(show, dashed, size, color);
      }

      //  Set Ui Texture Panel
      {
        auto &[technique] = scene_->GetComponent<TechniqueComponent>(entity);
        emit signal_handler_.SetTexturePanel((int)technique);
      }

      //  Set Ui Material Panel
      {
        auto &[shininess] = scene_->GetComponent<MaterialComponent>(entity);
        emit signal_handler_.SetMaterialPanel(shininess);
      }

      //  Set Ui PointLight Panel
      {
        bool sourceLight =
            scene_->EntityHasComponent<PointLightComponent>(entity);
        auto const &[ambient, diffuse, specular, constant, linear, quadrratic] =
            scene_->GetComponent<LightSettingsComponent>(entity);

        emit signal_handler_.SetPointLightPanel(sourceLight, ambient, diffuse,
                                                specular,
                                                {constant, linear, quadrratic});
      }

      //  Set Ui Axis Panel
      {
        bool axis = scene_->EntityHasComponent<AxisComponent>(entity);
        signal_handler_.SetAxisPanel(axis);
      }
    }
  }

  void UpdateTransformInfo(TransformComponent const &t) {
    for (auto &entity : entities_) {
      auto &transform = scene_->GetComponent<TransformComponent>(entity);
      transform = t;
    }
  }

  void UpdatePointInfo(PointSettingsComponent const &settings) {
    for (auto &entity : entities_) {
      auto &point = scene_->GetComponent<PointSettingsComponent>(entity);
      point = settings;
    }
  }

  void UpdateLineInfo(LineSettingsComponent const &settings) {
    for (auto &entity : entities_) {
      auto &line = scene_->GetComponent<LineSettingsComponent>(entity);
      line = settings;
    }
  }

  void UpdateAxisInfo(bool axis) {
    for (auto &entity : entities_) {
      axis ? scene_->AddComponent<AxisComponent>(entity)
           : scene_->RemoveComponent<AxisComponent>(entity);
    }
  }

  void UpdateTextureTechnique(TechniqueType type) {
    for (auto &entity : entities_) {
      auto &technique = scene_->GetComponent<TechniqueComponent>(entity);
      technique = {type};
    }
  }

  void UpdateTextureInfo(uint32_t textureID, TextureType type) {
    for (auto &entity : entities_) {
      auto &texture = scene_->GetComponent<TextureComponent>(entity);
      switch (type) {
      case TextureType::DIFFUSE:
        texture.diffuse = textureID;
        break;
      case TextureType::SPECULAR:
        texture.specular = textureID;
        break;
      case TextureType::NORMAL:
        texture.normal = textureID;
        break;
      case TextureType::HEIGHT:
        texture.height = textureID;
        break;
      default:
        assert(false &&
               "ObjectEditorSystem::UpdateTextureInfo: no such Texture Type");
        break;
      }
    }
  }

  void MakeLightSource(bool light) {
    for (auto &entity : entities_) {
      light ? scene_->AddComponent<PointLightComponent>(entity)
            : scene_->RemoveComponent<PointLightComponent>(entity);
    }
  }

  void UpdatePointLightInfo(LightSettingsComponent const &settings) {
    for (auto &entity : entities_) {
      auto &lightSettings =
          scene_->GetComponent<LightSettingsComponent>(entity);
      lightSettings = settings;
    }
  }

  void UpdateMaterialInfo(MaterialComponent const &value) {
    for (auto &entity : entities_) {
      auto &material = scene_->GetComponent<MaterialComponent>(entity);
      material = value;
    }
  }

  void DeleteObject() {
    for (auto &entity : entities_) {
      scene_->DestroyEntity(entity);
    }
  }

private:
  std::shared_ptr<Controller> scene_;
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();
};
} // namespace s21
