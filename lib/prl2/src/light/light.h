#ifndef _PRL2_LIGHT_H
#define _PRL2_LIGHT_H

#include <vector>

#include "core/isect.h"
#include "core/ray.h"
#include "core/spectrum.h"

namespace Prl2 {

// 正規化されたD65光源のSPDを返す
SPD D65Light();

class Light {
 public:
  Light(const SPD& _spd);

  // 光源上の点から出る分光放射輝度を返す
  virtual Real Le(const Ray& ray, const IntersectInfo& info) const = 0;

 private:
  SPD spd;  // 分光放射束
};

}  // namespace Prl2

#endif