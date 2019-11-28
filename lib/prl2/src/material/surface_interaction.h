#ifndef _PRL2_SURFACE_INTERACTION
#define _PRL2_SURFACE_INTERACTION

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

struct SurfaceInteraction {
  SurfaceInteraction(const Vec3& _wo_local, const Real& _lambda)
      : wo_local(_wo_local), lambda(_lambda){};

  const Vec3 wo_local;  // マテリアル座標系の出射ベクトル
  const Real lambda;    // 波長
};

}  // namespace Prl2

#endif