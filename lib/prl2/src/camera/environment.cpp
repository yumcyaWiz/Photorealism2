#include "camera/environment.h"

namespace Prl2 {

EnvironmentCamera::EnvironmentCamera(
    const std::shared_ptr<Film>& _film,
    const std::shared_ptr<Transform>& _localToWorld)
    : Camera(_film, _localToWorld){};

bool EnvironmentCamera::generateRay(const Real& u, const Real& v,
                                    Ray& ray) const {
  // 球面座標を計算
  const Real theta = u * PI_MUL_2;
  const Real phi = v * PI;

  // 球面座標からレイを生成
  ray.origin = Vec3(0);
  ray.direction = sphericalToCartesian(theta, phi);

  //ワールド座標系に変換
  ray = localToWorld->apply(ray);

  return true;
}

}  // namespace Prl2