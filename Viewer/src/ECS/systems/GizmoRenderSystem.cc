#include "GizmoRenderSystem.h"

#include <QVector4D>

#include "OpenGLDebug.h"
#include "Utils.h"

namespace s21 {

// struct rect {
//   int x0, y0, x1, y1;
//   int width() const { return x1 - x0; }
//   int height() const { return y1 - y0; }
//   linalg::aliases::int2 dims() const { return {width(), height()}; }
//   float aspect_ratio() const { return (float)width() / height(); }
// };

// struct camera {
//   float yfov, near_clip, far_clip;
//   linalg::aliases::float3 position;
//   float pitch, yaw;
//   linalg::aliases::float4 get_orientation() const {
//     return qmul(rotation_quat(linalg::aliases::float3(0, 1, 0), yaw),
//                 rotation_quat(linalg::aliases::float3(1, 0, 0), pitch));
//   }
//   linalg::aliases::float4x4 get_view_matrix() const {
//     return mul(rotation_matrix(qconj(get_orientation())),
//                translation_matrix(-position));
//   }
//   linalg::aliases::float4x4 get_projection_matrix(
//       const float aspectRatio) const {
//     return linalg::perspective_matrix(yfov, aspectRatio, near_clip,
//     far_clip);
//   }
//   linalg::aliases::float4x4 get_viewproj_matrix(const float aspectRatio)
//   const {
//     return mul(get_projection_matrix(aspectRatio), get_view_matrix());
//   }
// };

// struct ray {
//   linalg::aliases::float3 origin;
//   linalg::aliases::float3 direction;
// };

void GizmoRenderSystem::RenderGizmo(const tinygizmo::geometry_mesh &mesh) {
  static uint32_t VAO = 0;
  static uint32_t index_count = 0;
  static uint32_t vertex_count = 0;

  // if (VAO == 0) {
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(tinygizmo::geometry_vertex),
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
  // }

  Q_ASSERT(VAO && index_count && vertex_count);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
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

// ray get_ray_from_pixel(const linalg::aliases::float2 &pixel,
//                        const rect &viewport, const camera &cam) {
//   const float x = 2 * (pixel.x - viewport.x0) / viewport.width() - 1;
//   const float y = 1 - 2 * (pixel.y - viewport.y0) / viewport.height();

//   const linalg::aliases::float4x4 inv_view_proj =
//       inverse(cam.get_viewproj_matrix(viewport.aspect_ratio()));

//   const linalg::aliases::float4 p0 =
//       mul(inv_view_proj, linalg::aliases::float4(x, y, -1, 1));
//   const linalg::aliases::float4 p1 =
//       mul(inv_view_proj, linalg::aliases::float4(x, y, +1, 1));

//   return {cam.position, p1.xyz() * p0.w - p0.xyz() * p1.w};
// }

QVector3D GetRayFromPixel(QPoint pixel, Camera const &camera) {
  const float x = 2 * (float)(pixel.x() - 0) / (float)camera.width - 1;
  const float y = 1 - 2 * (float)(pixel.y() - 0) / (float)camera.height;

  QMatrix4x4 inveresed_proj_view =
      (camera.projection_matrix * camera.view_matrix).inverted();

  QVector4D p0 = inveresed_proj_view * QVector4D(x, y, -1, 1);
  QVector4D p1 = inveresed_proj_view * QVector4D(x, y, +1, 1);

  return QVector3D(p1) * p0.w() - QVector3D(p0) * p1.w();
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

  auto ray = GetRayFromPixel(input.mouse_pos, camera);
  // qDebug() << input.mouse_pos;
  // qDebug() << ray;
  // qDebug() << camera.front;

  gizmo_state.ray_direction = minalg::float3(ray.x(), ray.y(), ray.z());

  // optional flag to draw the gizmos at a constant screen-space scale
  // gizmo_state.screenspace_scale = 80.f;
  gizmo_state.mouse_left = input.left_button_pressed;

  gizmo_state.hotkey_local = input.keys[Qt::Key_L];
  gizmo_state.hotkey_scale = input.keys[Qt::Key_S];
  gizmo_state.hotkey_rotate = input.keys[Qt::Key_R];
  gizmo_state.hotkey_translate = input.keys[Qt::Key_T];
  gizmo_state.hotkey_ctrl = input.keys[Qt::Key_Control];

  // qDebug() << gizmo_state.hotkey_ctrl << gizmo_state.hotkey_rotate;
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

    auto name =
        QString::fromStdString("gizmo of entity" + std::to_string(entity));

    if (transform_gizmo(name.toStdString(), gizmo_ctx, xform)) {
      qDebug() << "First Gizmo Hovered...";
      if (xform != xform_last) {
        qDebug() << "First Gizmo Changed...";
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
