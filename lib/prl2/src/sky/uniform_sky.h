#ifndef _PRL2_UNIFORM_SKY_H
#define _PRL2_UNIFORM_SKY_H

#include "core/spectrum.h"
#include "sky/sky.h"

namespace Prl2 {

class UniformSky : public Sky {
 public:
  UniformSky(const SPD& _spd) : spd(_spd) {}

  Real getRadiance(const Ray& ray) const override {
    return spd.sample(ray.lambda);
  }

 private:
  SPD spd;
};

}  // namespace Prl2

#endif