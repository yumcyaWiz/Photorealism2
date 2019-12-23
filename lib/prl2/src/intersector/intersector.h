#ifndef INTERSECTOR_H
#define INTERSECTOR_H

#include <vector>

#include "core/isect.h"
#include "core/primitive.h"
#include "core/ray.h"

namespace Prl2 {

//与えられたレイとPrimitiveの集合の衝突計算を行うクラス
class Intersector {
 public:
  Intersector(){};

  void setPrimitives(const std::vector<std::shared_ptr<Primitive>>& _prims) {
    primitives = _prims;
  };

  // 初期化
  virtual bool initialize() const = 0;

  //与えられたレイとの衝突計算を行う
  virtual bool intersect(const Ray& ray, IntersectInfo& info) const = 0;

 protected:
  std::vector<std::shared_ptr<Primitive>> primitives;  // Primitiveの配列
};

}  // namespace Prl2

#endif