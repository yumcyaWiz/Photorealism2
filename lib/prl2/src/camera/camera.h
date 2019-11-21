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
  std::shared_ptr<Transform>
      localToWorld;  // ローカル座標系からワールド座標系へのTransform

  Camera(const std::shared_ptr<Film>& _film,
         const std::shared_ptr<Transform>& _localToWorld)
      : film(_film), localToWorld(_localToWorld){};

  void setLookAt(const Vec3& pos, const Vec3& lookat) {
    localToWorld = std::make_shared<Transform>(lookAt(pos, lookat));
  };

  //(u, v)におけるレイを生成する
  //(u, v)はフィルム面を[-1, -1] x [1, 1]で表している
  virtual bool generateRay(const Real& u, const Real& v, Ray& ray) const = 0;
};

}  // namespace Prl2
#endif