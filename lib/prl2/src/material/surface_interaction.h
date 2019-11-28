#ifndef _PRL2_SURFACE_INTERACTION
#define _PRL2_SURFACE_INTERACTION

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

struct SurfaceInteraction {
  SurfaceInteraction(const Vec3& _wo_local, const Real& _lambda,
                     const Real& _ior)
      : wo_local(_wo_local), lambda(_lambda), ior(_ior){};

  const Vec3 wo_local;  // マテリアル座標系の出射ベクトル
  const Real lambda;    // 波長
  const Real ior;       // 現在の媒質の屈折率
};

}  // namespace Prl2

#endif