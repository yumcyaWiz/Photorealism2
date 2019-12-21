#ifndef LINEAR_H
#define LINEAR_H

#include "intersector/intersector.h"

namespace Prl2 {

//全ての物体と衝突計算を行い、衝突距離が最も小さいものを返す
class LinearIntersector : public Intersector {
 public:
  LinearIntersector(){};

  bool intersect(const Ray& ray, IntersectInfo& info) const override {
    bool hit = false;

    Real t = ray.tmax;
    IntersectInfo info_tmp;
    for (const auto& prim : primitives) {
      if (prim->intersect(ray, info_tmp)) {
        //衝突距離が最も小さいものを選ぶ
        if (info_tmp.t < t) {
          t = info_tmp.t;
          info = info_tmp;
          hit = true;
        }
      }
    }

    return hit;
  };
};

}  // namespace Prl2

#endif