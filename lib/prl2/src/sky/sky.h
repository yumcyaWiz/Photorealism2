#ifndef _PRL2_SKY_H
#define _PRL2_SKY_H

#include "core/ray.h"

namespace Prl2 {

class Sky {
 public:
  Sky(){};

  // レイの方向から来る放射輝度を計算して返す
  virtual Real getRadiance(const Ray& ray) const = 0;
};

};  // namespace Prl2

#endif