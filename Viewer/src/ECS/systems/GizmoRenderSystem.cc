#include "GizmoRenderSystem.h"

#include "OpenGLDebug.h"
#include "Utils.h"

namespace s21 {

void GizmoRenderSystem::RenderGizmo(const tinygizmo::geometry_mesh &mesh) {
  static uint32_t VAO = 0;
  static uint32_t index_count = 0;
  static uint32_t vertex_count = 0;

  if (VAO == 0) {
    uint32_t VBO = 0, EBO = 0;

    auto &[verticies, triangles] = mesh;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(tinygizmo::geometry_vertex) * verticies.size(),
                 verticies.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(minalg::uint3) * triangles.size(), triangles.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(tinygizmo::geometry_vertex),
        (void *)offsetof(tinygizmo::geometry_vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(tinygizmo::geometry_vertex),
                          (void *)offsetof(tinygizmo::geometry_vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                          sizeof(tinygizmo::geometry_vertex),
                          (void *)offsetof(tinygizmo::geometry_vertex, color));

    index_count = triangles.size() * 3;
    vertex_count = verticies.size();
  }

  Q_ASSERT(VAO && index_count && vertex_count);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void GizmoRenderSystem::Init(ECS_Controller *scene,
                             TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;

  gizmo_ctx.render = [&](const tinygizmo::geometry_mesh &r) { RenderGizmo(r); };
}

tinygizmo::rigid_transform ToRigidTransform(const Transform &transform) {
  tinygizmo::rigid_transform xform;

  xform.position = {transform.translation.x(), transform.translation.y(),
                    transform.translation.z()};

  xform.orientation = {transform.rotation.x(), transform.rotation.y(),
                       transform.rotation.z(), transform.rotation.scalar()};

  xform.scale = {transform.scale.x(), transform.scale.y(), transform.scale.z()};

  return xform;
}

Transform FromRigidTransform(tinygizmo::rigid_transform const &xform) {
  Transform transform;

  transform.translation = {xform.position.x, xform.position.y,
                           xform.position.z};
  transform.rotation = {xform.orientation.w, xform.orientation.x,
                        xform.orientation.y, xform.orientation.z};
  transform.scale = {xform.scale.x, xform.scale.y, xform.scale.z};

  return transform;
}

void UpdateState(tinygizmo::gizmo_application_state &gizmo_state,
                 const Camera &camera, Input &input) {
  gizmo_state.viewport_size = minalg::float2(camera.width, camera.height);
  gizmo_state.cam.near_clip = camera.near_clip;
  gizmo_state.cam.far_clip = camera.far_clip;
  gizmo_state.cam.yfov = camera.zoom;
  gizmo_state.cam.position = minalg::float3(
      camera.position.x(), camera.position.y(), camera.position.z());

  auto q = QQuaternion::fromEulerAngles(camera.pitch, camera.yaw, 0.0f);

  gizmo_state.cam.orientation = minalg::float4(q.x(), q.y(), q.z(), q.scalar());

  gizmo_state.ray_origin = gizmo_state.cam.position;
  gizmo_state.ray_direction =
      minalg::float3(camera.front.x(), camera.front.y(), camera.front.z());
  // gizmo_state.screenspace_scale = 80.f; // optional flag to draw the gizmos
  // at a constant screen-space scale

  gizmo_state.hotkey_ctrl = input.keys[Qt::Key_Control];
  gizmo_state.hotkey_local = input.keys[Qt::Key_L];
  gizmo_state.hotkey_translate = input.keys[Qt::Key_T];
  gizmo_state.hotkey_rotate = input.keys[Qt::Key_R];
  gizmo_state.hotkey_scale = input.keys[Qt::Key_S];

  gizmo_state.mouse_left = input.left_button_pressed;
}

void GizmoRenderSystem::Update() {
  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));
  auto &input = scene_->GetComponent<Input>(Utils::GetInput(scene_));

  UpdateState(gizmo_state, camera, input);
  gizmo_ctx.update(gizmo_state);

  glEnable(GL_BLEND);
  glClear(GL_DEPTH_BUFFER_BIT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto entity : entities_) {
    auto &transform = scene_->GetComponent<Transform>(entity);

    technique_->Enable(TechniqueType::GIZMO);
    technique_->setMVP(camera.projection_matrix, camera.view_matrix,
                       transform.GetModelMatrix());

    tinygizmo::rigid_transform xform, xform_last;
    xform = xform_last = ToRigidTransform(transform);

    qDebug() << "First Gizmo render...";

    if (transform_gizmo("first-example-gizmo", gizmo_ctx, xform)) {
      qDebug() << "First Gizmo Hovered...";
      if (xform != xform_last) qDebug() << "First Gizmo Changed...";
      transform = FromRigidTransform(xform);
      // qDebug() << tmp.translation << tmp.rotation << tmp.scale;
    }

    gizmo_ctx.draw();
  }

  glDisable(GL_BLEND);
}

}  // namespace s21
