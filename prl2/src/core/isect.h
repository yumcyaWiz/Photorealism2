#ifndef ISECT_H
#define ISECT_H

#include <iostream>
#include <limits>

#include "core/ray.h"
#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

struct IntersectInfo {
  Real t;          //衝突距離
  Vec3 hitPos;     //衝突位置
  Vec3 hitNormal;  //法線
  Vec3 dpdu;       //接線
  Vec3 dpdv;       // 陪法線

  IntersectInfo()
      : t(std::numeric_limits<Real>::max()),
        hitPos(Vec3()),
        hitNormal(Vec3()){};
  IntersectInfo(const Real& _t, const Vec3& _hitPos, const Vec3& _hitNormal)
      : t(_t), hitPos(_hitPos), hitNormal(_hitNormal){};
};

inline std::ostream& operator<<(std::ostream& stream,
                                const IntersectInfo& info) {
  stream << "t: " << info.t << std::endl;
  stream << "hitPos: " << info.hitPos << std::endl;
  stream << "hitNormal: " << info.hitNormal << std::endl;
  return stream;
}

}  // namespace Prl2

#endif