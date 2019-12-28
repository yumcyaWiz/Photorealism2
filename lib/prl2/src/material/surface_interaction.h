#ifndef _PRL2_SURFACE_INTERACTION
#define _PRL2_SURFACE_INTERACTION

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

struct SurfaceInteraction {
  SurfaceInteraction(){};

  Vec3 s;         // binormal
  Vec3 t;         // tangent
  Vec3 wo_local;  // マテリアル座標系の出射ベクトル
  Vec3 wi_local;  // マテリアル座標系の入射ベクトル
  Real lambda;    // 波長
};

}  // namespace Prl2

#endif