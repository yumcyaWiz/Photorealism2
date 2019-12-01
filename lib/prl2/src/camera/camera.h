#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include "camera/film.h"
#include "core/ray.h"
#include "core/transform.h"
#include "core/type.h"

namespace Prl2 {

//カメラを表すクラス
//カメラ座標系はフィルム中心を原点とし、右が+X, 上が+Y, 前が-Zで定義される
class Camera {
 public:
  const std::shared_ptr<Film> film;  // フィルム

  Camera(const std::shared_ptr<Film>& _film,
         const std::shared_ptr<Transform>& _localToWorld)
      : film(_film), localToWorld(_localToWorld){};

  // LookAt Transformを設定する
  void setLookAt(const Vec3& pos, const Vec3& lookat) {
    localToWorld = std::make_shared<Transform>(lookAt(pos, lookat));
  };

  // Camera Positionを動かす
  void moveCamera(const Vec3& pos_diff) {
    localToWorld->mat.m[0][3] += pos_diff.x();
    localToWorld->mat.m[1][3] += pos_diff.y();
    localToWorld->mat.m[2][3] += pos_diff.y();
  };

  //(u, v)におけるレイを生成する
  //(u, v)はフィルム面を[-1, -1] x [1, 1]で表している
  virtual bool generateRay(const Real& u, const Real& v, Ray& ray) const = 0;

 protected:
  std::shared_ptr<Transform>
      localToWorld;  // ローカル座標系からワールド座標系へのTransform
};

}  // namespace Prl2
#endif