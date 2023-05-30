#include "GizmoRenderSystem.h"

#include <QVector4D>

#include "Input.h"
#include "OpenGLDebug.h"
#include "Utils.h"

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

QVector3D GetRayFromPixel(Camera const &camera) {
  QPoint pixel = Input::MousePosition();

  const float x = 2 * (float)(pixel.x() - 0) / (float)camera.width - 1;
  const float y = 1 - 2 * (float)(pixel.y() - 0) / (float)camera.height;

  QMatrix4x4 inveresed_proj_view =
      (camera.projection_matrix * camera.view_matrix).inverted();

  QVector4D p0 = inveresed_proj_view * QVector4D(x, y, -1, 1);
  QVector4D p1 = inveresed_proj_view * QVector4D(x, y, +1, 1);

  return QVector3D(p1) * p0.w() - QVector3D(p0) * p1.w();
}

void UpdateState(tinygizmo::gizmo_application_state &gizmo_state,
                 const Camera &camera) {
  auto q = QQuaternion::fromEulerAngles(camera.pitch, -(camera.yaw + 90), 0.0f);
  auto ray = GetRayFromPixel(camera);

  gizmo_state.cam.far_clip = camera.far_clip;
  gizmo_state.cam.near_clip = camera.near_clip;
  gizmo_state.ray_origin = gizmo_state.cam.position;
  gizmo_state.ray_direction = minalg::float3(ray.x(), ray.y(), ray.z());
  gizmo_state.viewport_size = minalg::float2(camera.width, camera.height);
  gizmo_state.cam.orientation = minalg::float4(q.x(), q.y(), q.z(), q.scalar());
  gizmo_state.cam.yfov = camera.zoom;
  gizmo_state.cam.position = minalg::float3(
      camera.position.x(), camera.position.y(), camera.position.z());

  // optional flag to draw the gizmos at a constant screen-space scale
  // gizmo_state.screenspace_scale = 80.f;

  gizmo_state.mouse_left = Input::IsMouseButtonPressed(Qt::LeftButton);
  gizmo_state.hotkey_local = Input::IsKeyPressed(Qt::Key_L);
  gizmo_state.hotkey_scale = Input::IsKeyPressed(Qt::Key_S);
  gizmo_state.hotkey_rotate = Input::IsKeyPressed(Qt::Key_R);
  gizmo_state.hotkey_translate = Input::IsKeyPressed(Qt::Key_T);
  gizmo_state.hotkey_ctrl = Input::IsKeyPressed(Qt::Key_Control);
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
  auto &camera = scene_->GetComponent<Camera>(Utils::GetCamera(scene_));

  UpdateState(gizmo_state, camera);
  gizmo_ctx.update(gizmo_state);

  glEnable(GL_BLEND);
  glClear(GL_DEPTH_BUFFER_BIT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto entity : entities_) {
    auto &transform = scene_->GetComponent<Transform>(entity);

    technique_->Enable(TechniqueType::GIZMO);
    technique_->setMVP(camera.projection_matrix, camera.view_matrix, {});

    tinygizmo::rigid_transform xform, xform_last;
    xform = xform_last = ToRigidTransform(transform);

    auto name =
        QString::fromStdString("gizmo of entity" + std::to_string(entity));

    if (transform_gizmo(name.toStdString(), gizmo_ctx, xform)) {
      qDebug() << name << "Hovered...";
      if (xform != xform_last) {
        qDebug() << name << "Changed...";
        transform = FromRigidTransform(xform);
      }
    } else {
      qDebug() << name + " not hovered...";
    }

    gizmo_ctx.draw();
  }

  glDisable(GL_BLEND);
}

}  // namespace s21
