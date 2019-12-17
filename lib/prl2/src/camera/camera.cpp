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

  lookat[0] = pos[0] - localToWorld->mat.m[0][2];
  lookat[1] = pos[1] - localToWorld->mat.m[1][2];
  lookat[2] = pos[2] - localToWorld->mat.m[2][2];
}

void Camera::getDirections(Vec3& right, Vec3& up, Vec3& forward) const {
  const Mat4& mat = localToWorld->mat;
  right[0] = mat.m[0][0];
  right[1] = mat.m[1][0];
  right[2] = mat.m[2][0];

  up[0] = mat.m[0][1];
  up[1] = mat.m[1][1];
  up[2] = mat.m[2][1];

  forward[0] = mat.m[0][2];
  forward[1] = mat.m[1][2];
  forward[2] = mat.m[2][2];
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
  // カメラの位置を入手
  Vec3 pos, lookat;
  getLookAt(pos, lookat);

  // 方向ベクトルを入手
  Vec3 right, up, forward;
  getDirections(right, up, forward);

  // Transformを原点に戻してから、それぞれの軸ごとに回転させ、元に戻す
  *localToWorld = translate(pos) * rotate(r.x(), right) * rotate(r.y(), up) *
                  translate(-pos) * (*localToWorld);
}

void Camera::setCameraAroundLookAt(const Real& theta, const Real& phi) {
  Vec3 pos, lookat;
  getLookAt(pos, lookat);

  const Real r = length(lookat - pos);
  const Real x = r * std::cos(phi) * std::sin(theta);
  const Real y = r * std::cos(theta);
  const Real z = r * std::sin(phi) * std::sin(theta);

  Vec3 right, up, forward;
  getDirections(right, up, forward);

  pos = lookat + (x * right + y * up + z * forward);
  setLookAt(pos, lookat);
}

Vec2 Camera::sampleFilm(int i, int j, Sampler& sampler) const {
  const Real u = (2.0f * (i + sampler.getNext()) - film->width) / film->width;
  const Real v = (2.0f * (j + sampler.getNext()) - film->height) / film->height;
  return Vec2(0.5f * film->width_length * u, 0.5f * film->height_length * v);
}

void Camera::getTransformMatrix(Mat4& mat) const { mat = localToWorld->mat; }

}  // namespace Prl2
