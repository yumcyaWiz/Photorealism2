#ifndef _PRL2_SURFACE_INTERACTION
#define _PRL2_SURFACE_INTERACTION

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

struct SurfaceInteraction {
  SurfaceInteraction(){};

  Vec3 wo_local;  // マテリアル座標系の出射ベクトル
  Real lambda;    // 波長
  Real ior;       // 現在の媒質の屈折率
  Vec3 wi_local;  // マテリアル座標系の入射ベクトル
};

}  // namespace Prl2

#endif