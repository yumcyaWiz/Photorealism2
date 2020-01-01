#ifndef ISECT_H
#define ISECT_H

#include <iostream>
#include <limits>

#include "core/ray.h"
#include "core/type.h"
#include "core/vec2.h"
#include "core/vec3.h"

namespace Prl2 {

// 前方宣言
class Primitive;

struct IntersectInfo {
  Real t;                         // 衝突距離
  Vec3 hitPos;                    // 衝突位置
  Vec3 hitNormal;                 // 法線
  Vec2 uv;                        // UV座標
  const Primitive* hitPrimitive;  // 衝突Primitiveへのポインタ

  IntersectInfo()
      : t(std::numeric_limits<Real>::max()),
        hitPos(Vec3()),
        hitNormal(Vec3()),
        uv(Vec2()),
        hitPrimitive(nullptr){};
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