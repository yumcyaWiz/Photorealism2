#ifndef SHAPE_H
#define SHAPE_H

#include <memory>

#include "isect.h"
#include "ray.h"
#include "transform.h"

//物体の形状を表現するクラス
//衝突計算は物体中心を原点とするローカル座標で行われる
class Shape {
 public:
  Shape(){};

  //受け取ったレイとの衝突計算を行い、結果をinfoに格納する
  virtual bool intersect(const Ray& ray, IntersectInfo& info) const = 0;
};

#endif