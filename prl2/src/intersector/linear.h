#ifndef LINEAR_H
#define LINEAR_H

#include "intersector/intersector.h"

namespace Prl2 {

//全ての物体と衝突計算を行い、衝突距離が最も小さいものを返す
class LinearIntersector : public Intersector {
 public:
  LinearIntersector(){};

  bool intersect(const Ray& ray, IntersectInfo& info) const {
    bool hit = false;

    float t = ray.tmax;
    for (const auto& prim : primitives) {
      if (prim->intersect(ray, info)) {
        //衝突距離が最も小さいものを選ぶ
        if (info.t < t) {
          t = info.t;
          hit = true;
        }
      }
    }

    return hit;
  };
};

}  // namespace Prl2

#endif