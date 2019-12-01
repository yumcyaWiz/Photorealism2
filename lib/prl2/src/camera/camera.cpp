#include "camera.h"

namespace Prl2 {

Camera::Camera(const std::shared_ptr<Film>& _film,
               const std::shared_ptr<Transform>& _localToWorld)
    : film(_film), localToWorld(_localToWorld) {}

void Camera::setLookAt(const Vec3& pos, const Vec3& lookat) {
  localToWorld = std::make_shared<Transform>(lookAt(pos, lookat));
}

void Camera::getLookAt(Vec3& pos, Vec3& lookat) const {
  pos[0] = localToWorld->mat.m[0][3];
  pos[1] = localToWorld->mat.m[1][3];
  pos[2] = localToWorld->mat.m[2][3];

  lookat[0] = pos[0] + localToWorld->mat.m[0][2];
  lookat[1] = pos[1] + localToWorld->mat.m[1][2];
  lookat[2] = pos[2] + localToWorld->mat.m[2][2];
}

void Camera::moveCamera(const Vec3& pos_diff) {
  Mat4& mat = localToWorld->mat;

  // Right
  mat.m[0][3] += pos_diff.x() * mat.m[0][0];
  mat.m[1][3] += pos_diff.x() * mat.m[1][0];
  mat.m[2][3] += pos_diff.x() * mat.m[2][0];

  // Up
  mat.m[0][3] += pos_diff.y() * mat.m[0][1];
  mat.m[1][3] += pos_diff.y() * mat.m[1][1];
  mat.m[2][3] += pos_diff.y() * mat.m[2][1];

  // Forward
  mat.m[0][3] += pos_diff.z() * mat.m[0][2];
  mat.m[1][3] += pos_diff.z() * mat.m[1][2];
  mat.m[2][3] += pos_diff.z() * mat.m[2][2];
}

void Camera::rotateCamera(const Vec3& r) {
  const Mat4& mat = localToWorld->mat;

  // カメラの方向ベクトルを入手
  const Vec3 right(mat.m[0][0], mat.m[1][0], mat.m[2][0]);
  const Vec3 up(mat.m[1][0], mat.m[1][1], mat.m[1][2]);
  const Vec3 forward(mat.m[2][0], mat.m[2][1], mat.m[2][2]);

  Vec3 pos, lookat;
  getLookAt(pos, lookat);

  // それぞれの方向ベクトルを中心にlookatを回転する行列を生成
  const Transform rotate_x = rotate(r.x(), right);
  const Transform rotate_y = rotate(r.y(), up);
  const Transform rotate_z = rotate(r.z(), forward);

  // LookAtを回転
  Vec3 f = -forward;
  f = rotate_x.applyDirection(f);
  f = rotate_y.applyDirection(f);
  f = rotate_z.applyDirection(f);

  // LookAtをセット
  setLookAt(pos, pos + f);
}

}  // namespace Prl2
