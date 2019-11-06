#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include "camera/film.h"
#include "core/ray.h"
#include "core/transform.h"
#include "core/type.h"

namespace Prl2 {

//カメラを表すクラス
//カメラ座標系は右が+X, 前が+Y, 上が+Zで定義される
class Camera {
 public:
  Camera(const std::shared_ptr<Film>& _film) : film(_film){};

  //(u, v)におけるレイを生成する
  //(u, v)はフィルム面を[-1, -1] x [1, 1]で表している
  virtual bool generateRay(const Real& u, const Real& v, Ray& ray) const = 0;

 protected:
  const std::shared_ptr<Transform>
      localToWorld;  //ローカル座標系からワールド座標系へのTransform
  const std::shared_ptr<Film> film;  //フィルム
};

}  // namespace Prl2
#endif