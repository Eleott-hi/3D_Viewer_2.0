#pragma once

#include <vector>

#include "ECS_Controller.h"
#include "TechniqueStrategy.h"

namespace s21 {

class Model {
 public:
  Model() = default;
  ~Model() = default;

  void AddMesh(EntityID mesh_id) { meshes_.push_back(mesh_id); }
  void DeleteMesh(EntityID mesh_id) {
    meshes_.erase(std::find(meshes_.begin(), meshes_.end(), mesh_id));
  }

  void SetController(ECS_Controller *controller) { scene_ = controller; }
  void Draw(TechniqueStrategy *technique) {
    for (auto entity : meshes_) {
      auto &mesh = scene_->GetComponent<Mesh>(entity);
      auto shader = scene_->GetComponent<Shader>(entity).type;
      auto const &material = scene_->GetComponent<Material>(entity);
      auto const &transform = scene_->GetComponent<Transform>(entity);

      technique->Enable(shader);
      technique->Clear();
      technique->SetMaterial(material);
      //   technique->SetModel(transform.GetModelMatrix());
      technique->SetMVP({}, {}, transform.GetModelMatrix());
    }
  }

 private:
  std::vector<EntityID> meshes_;
  ECS_Controller *scene_;
};
}  // namespace s21
