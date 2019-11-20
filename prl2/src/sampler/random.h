#ifndef RANDOM_H
#define RANDOM_H
#include "sampler/rng.h"
#include "sampler/sampler.h"

namespace Prl2 {

// ただの乱数を返すSampler
class RandomSampler : public Sampler {
 public:
  RandomSampler(){};
  RandomSampler(uint64_t seed) : rng(RNG(seed)){};

  void setSeed(uint64_t seed) { rng.setSeed(seed); };

  Real getNext() { return rng.uniformFloat(); }
  Vec2 getNext2D() { return Vec2(rng.uniformFloat(), rng.uniformFloat()); };

 private:
  RNG rng;
};

}  // namespace Prl2

#endif