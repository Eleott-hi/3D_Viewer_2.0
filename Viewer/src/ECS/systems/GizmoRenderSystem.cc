#include "GizmoRenderSystem.h"

#include "Input.h"
#include "OpenGLDebug.h"
#include "Utils.h"
#include "gizmo/tiny-gizmo.h"

namespace s21 {

void RenderGizmo(QOpenGLExtraFunctions *f,
                 const tinygizmo::geometry_mesh &mesh) {
  uint32_t VAO = 0, VBO = 0, EBO = 0;
  uint32_t index_count = 0, vertex_count = 0;

  auto &[verticies, triangles] = mesh;

  OPENGL_DEBUG(f->glGenVertexArrays(1, &VAO));
  OPENGL_DEBUG(f->glGenBuffers(1, &VBO));
  OPENGL_DEBUG(f->glGenBuffers(1, &EBO));

  OPENGL_DEBUG(f->glBindVertexArray(VAO));

  OPENGL_DEBUG(f->glBindBuffer(GL_ARRAY_BUFFER, VBO));
  OPENGL_DEBUG(f->glBufferData(
      GL_ARRAY_BUFFER, sizeof(tinygizmo::geometry_vertex) * verticies.size(),
      verticies.data(), GL_DYNAMIC_DRAW));

  OPENGL_DEBUG(f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
  OPENGL_DEBUG(f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                               sizeof(minalg::uint3) * triangles.size(),
                               triangles.data(), GL_DYNAMIC_DRAW));

  OPENGL_DEBUG(f->glEnableVertexAttribArray(0));
  OPENGL_DEBUG(f->glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, sizeof(tinygizmo::geometry_vertex),
      (void *)offsetof(tinygizmo::geometry_vertex, position)));

  OPENGL_DEBUG(f->glEnableVertexAttribArray(1));
  OPENGL_DEBUG(f->glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, sizeof(tinygizmo::geometry_vertex),
      (void *)offsetof(tinygizmo::geometry_vertex, normal)));

  OPENGL_DEBUG(f->glEnableVertexAttribArray(2));
  OPENGL_DEBUG(f->glVertexAttribPointer(
      2, 4, GL_FLOAT, GL_FALSE, sizeof(tinygizmo::geometry_vertex),
      (void *)offsetof(tinygizmo::geometry_vertex, color)));

  index_count = triangles.size() * 3;
  vertex_count = verticies.size();

  Q_ASSERT(VAO && index_count && vertex_count);

  OPENGL_DEBUG(f->glBindVertexArray(VAO));
  OPENGL_DEBUG(
      f->glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0));
  OPENGL_DEBUG(f->glBindVertexArray(0));

  OPENGL_DEBUG(f->glDeleteBuffers(1, &VBO));
  OPENGL_DEBUG(f->glDeleteBuffers(1, &EBO));
  OPENGL_DEBUG(f->glDeleteVertexArrays(1, &VAO));
}

Transform FromMatrixToTransform(QMatrix4x4 const &matrix) {
  QVector3D scale = {matrix.column(0).toVector3D().length(),
                     matrix.column(1).toVector3D().length(),
                     matrix.column(2).toVector3D().length()};
  QVector3D translation = matrix.column(3).toVector3D();

  QMatrix4x4 m_scale, m_translate, m_rotate;
  m_scale.scale(scale);
  m_translate.translate(translation);
  m_rotate = m_translate.inverted() * matrix * m_scale.inverted();

  QQuaternion rotation =
      QQuaternion::fromRotationMatrix(m_rotate.toGenericMatrix<3, 3>());

  return {translation, rotation, scale};
}

tinygizmo::rigid_transform ToRigidTransform(Transform const &transform) {
  auto const [sx, sy, sz] = transform.scale;
  auto const [tx, ty, tz] = transform.translation;
  auto const [rx, ry, rz, rw] = transform.rotation.toVector4D();

  return {{rx, ry, rz, rw}, {tx, ty, tz}, {sx, sy, sz}};
}

Transform FromRigidTransform(tinygizmo::rigid_transform const &transform) {
  auto const [sx, sy, sz] = transform.scale;
  auto const [tx, ty, tz] = transform.position;
  auto const [rx, ry, rz, rw] = transform.orientation;

  return {{tx, ty, tz}, {rw, rx, ry, rz}, {sx, sy, sz}};
}

QVector3D GetRayFromPixel(Camera const &camera) {
  QPoint pixel = Input::MousePosition();

  const float x = 2 * (float)(pixel.x() - 0) / (float)camera.width - 1;
  const float y = 1 - 2 * (float)(pixel.y() - 0) / (float)camera.height;

  QMatrix4x4 inveresed_proj_view =
      (camera.projection_ * camera.view_).inverted();

  QVector4D p0 = inveresed_proj_view * QVector4D(x, y, -1, 1);
  QVector4D p1 = inveresed_proj_view * QVector4D(x, y, +1, 1);

  return QVector3D(p1) * p0.w() - QVector3D(p0) * p1.w();
}

tinygizmo::gizmo_application_state GetTinygizmoState(const Camera &camera) {
  auto q = QQuaternion::fromEulerAngles(camera.pitch, -(camera.yaw + 90), 0.0f);
  auto [rx, ry, rz] = GetRayFromPixel(camera);
  auto [px, py, pz] = camera.position;
  auto [ox, oy, oz, ow] = q.toVector4D();

  tinygizmo::gizmo_application_state gizmo_state;
  tinygizmo::camera_parameters cam = {
      camera.zoom,
      camera.near_clip,
      camera.far_clip,
      minalg::float3{px, py, pz},
      minalg::float4{ox, oy, oz, ow},
  };

  gizmo_state.cam = cam;
  gizmo_state.ray_origin = gizmo_state.cam.position;
  gizmo_state.ray_direction = minalg::float3(rx, ry, rz);
  gizmo_state.viewport_size = minalg::float2(camera.width, camera.height);

  gizmo_state.hotkey_local = Input::IsKeyPressed(Qt::Key_L);
  gizmo_state.hotkey_scale = Input::IsKeyPressed(Qt::Key_S);
  gizmo_state.hotkey_rotate = Input::IsKeyPressed(Qt::Key_R);
  gizmo_state.hotkey_translate = Input::IsKeyPressed(Qt::Key_T);
  gizmo_state.hotkey_ctrl = Input::IsKeyPressed(Qt::Key_Control);
  gizmo_state.mouse_left = gizmo_state.hotkey_ctrl
                               ? Input::IsMouseButtonPressed(Qt::LeftButton)
                               : false;

  // optional flag to draw the gizmos at a constant screen-space scale
  // gizmo_state.screenspace_scale = 80.f;

  return gizmo_state;
}

void GizmoRenderSystem::Init(ECS_Controller *scene,
                             TechniqueStrategy *technique) {
  scene_ = scene;
  technique_ = technique;

  gizmo_ctx.render = [&](const tinygizmo::geometry_mesh &r) {
    RenderGizmo(this, r);
  };
}

void GizmoRenderSystem::Update() {
  std::unordered_map<int, QMatrix4x4> matricies;
  std::function<QMatrix4x4(EntityID)> get_model_matrix = [&](EntityID entity) {
    if (matricies.find(entity) == matricies.end() &&
        scene_->EntityHasComponent<Transform>(entity)) {
      auto &transform = scene_->GetComponent<Transform>(entity);
      auto &hierarchy = scene_->GetComponent<HierarchyComponent>(entity);

      matricies[entity] =
          get_model_matrix(hierarchy.parent) * transform.GetModelMatrix();
    }

    return matricies[entity];
  };

  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));
  gizmo_ctx.update(GetTinygizmoState(camera));

  technique_->Enable(TechniqueType::GIZMO);
  technique_->SetModelMatrix({});

  glEnable(GL_BLEND);
  glClear(GL_DEPTH_BUFFER_BIT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto entity : entities_) {
    auto &transform = scene_->GetComponent<Transform>(entity);
    auto &hierarchy = scene_->GetComponent<HierarchyComponent>(entity);
    auto parent_matrix = get_model_matrix(hierarchy.parent);

    gizmo_ctx.parent_matrix = {
        {parent_matrix(0, 0), parent_matrix(1, 0), parent_matrix(2, 0),
         parent_matrix(3, 0)},
        {parent_matrix(0, 1), parent_matrix(1, 1), parent_matrix(2, 1),
         parent_matrix(3, 1)},
        {parent_matrix(0, 2), parent_matrix(1, 2), parent_matrix(2, 2),
         parent_matrix(3, 2)},
        {parent_matrix(0, 3), parent_matrix(1, 3), parent_matrix(2, 3),
         parent_matrix(3, 3)},
    };

    tinygizmo::rigid_transform xform, xform_last;
    xform = xform_last = ToRigidTransform(transform);

    auto name = QString::fromStdString(  //
        "gizmo of entity " + std::to_string(entity));

    if (transform_gizmo(name.toStdString(), gizmo_ctx, xform)) {
      qDebug() << name << "Hovered...";
      if (xform != xform_last) {
        qDebug() << name << "Changed...";
        transform = FromRigidTransform(xform);
      }
    }

    gizmo_ctx.draw();
  }

  glDisable(GL_BLEND);
}

}  // namespace s21
