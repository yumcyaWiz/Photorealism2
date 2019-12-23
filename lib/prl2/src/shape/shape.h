#ifndef SHAPE_H
#define SHAPE_H

#include <memory>

#include "core/isect.h"
#include "core/ray.h"
#include "core/transform.h"
#include "sampler/sampler.h"

namespace Prl2 {

// 物体の形状を表現するクラス
// 衝突計算は物体中心を原点とするローカル座標で行われる
// スケール変換によりローカル座標でのレイの長さは1にならないことに注意
// 正規化してしまうと逆変換した際にレイが元の長さに戻らないことにも注意
class Shape {
 public:
  Shape(){};

  // 受け取ったレイとの衝突計算を行い、結果をinfoに格納する
  // ray : ローカル座標系のレイ
  // info : ローカル座標系の衝突情報
  virtual bool intersect(const Ray& ray, IntersectInfo& info) const = 0;

  // 表面上の点をサンプリングする
  virtual void samplePoint(Sampler& sampler, Vec3& p, Real& pdf_area) const = 0;
};

}  // namespace Prl2

#endif