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

}  // namespace Prl2
