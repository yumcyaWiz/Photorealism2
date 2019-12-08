#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include "camera/film.h"
#include "core/ray.h"
#include "core/transform.h"
#include "core/type.h"
#include "core/vec2.h"
#include "sampler/sampler.h"

namespace Prl2 {

//カメラを表すクラス
//カメラ座標系はフィルム中心を原点とし、右が+X, 上が+Y, 前が-Zで定義される
class Camera {
 public:
  const std::shared_ptr<Film> film;  // フィルム

  Camera(const std::shared_ptr<Film>& _film,
         const std::shared_ptr<Transform>& _localToWorld);

  // LookAt Transformを設定する
  void setLookAt(const Vec3& pos, const Vec3& lookat);

  // LookAt Transformからpos, lookatを復元する
  void getLookAt(Vec3& pos, Vec3& lookat) const;

  // LookAt TransformkからRight, Up, Forwardの方向ベクトルを入手
  void getDirections(Vec3& right, Vec3& up, Vec3& forward) const;

  // Camera Positionを動かす
  void moveCamera(const Vec3& pos_diff);

  // Cameraをそれぞれの軸を中心に回転させる
  void rotateCamera(const Vec3& r);

  // CameraをLookAtを中心とした球面上の位置にセットする
  void setCameraAroundLookAt(const Real& theta, const Real& phi);

  // 画素(i, j)内のフィルム上の点をサンプリングする
  Vec2 sampleFilm(int i, int j, Sampler& sampler) const;

  //フィルム上の点pFilmにおけるレイを生成する
  // pFilmはフィルム上の物理的位置である
  virtual bool generateRay(const Vec2& pFilm, Sampler& sampler, Ray& ray,
                           Real& cos, Real& pdf) const = 0;

 protected:
  std::shared_ptr<Transform>
      localToWorld;  // ローカル座標系からワールド座標系へのTransform
};

}  // namespace Prl2
#endif