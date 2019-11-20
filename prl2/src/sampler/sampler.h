#ifndef SAMPLER_H
#define SAMPLER_H

#include <cstdint>

#include "core/type.h"
#include "core/vec2.h"
#include "core/vec3.h"

namespace Prl2 {

// 乱数を生成するクラス
class Sampler {
 public:
  Sampler(){};

  //シード値を設定する
  virtual void setSeed(uint64_t seed) = 0;

  //次の次元の乱数を入手
  virtual Real getNext() = 0;

  //次の次元の乱数を２つ入手
  virtual Vec2 getNext2D() = 0;
};

}  // namespace Prl2

#endif