#include "intersector/embree.h"

namespace Prl2 {

void RTCErrorFunction(void* userPtr, RTCError code, const char* str) {
  fprintf(stderr, "error %d: %s", code, str);
}

void RTCUserGeometryIntersect(const RTCIntersectFunctionNArguments* args) {
  const Geometry* geometry =
      reinterpret_cast<const Geometry*>(args->geometryUserPtr);

  // compute ray
  RTCRayN* rayn = RTCRayHitN_RayN(args->rayhit, args->N);
  const float ox = RTCRayN_org_x(rayn, args->N, 0);
  const float oy = RTCRayN_org_y(rayn, args->N, 0);
  const float oz = RTCRayN_org_z(rayn, args->N, 0);
  const float dx = RTCRayN_dir_x(rayn, args->N, 0);
  const float dy = RTCRayN_dir_y(rayn, args->N, 0);
  const float dz = RTCRayN_dir_z(rayn, args->N, 0);
  const Ray ray(Vec3(ox, oy, oz), Vec3(dx, dy, dz));

  // intersect
  IntersectInfo info;
  bool is_hit = geometry->intersect(ray, info);

  // set intersect info
  RTCHitN* hitn = RTCRayHitN_HitN(args->rayhit, args->N);
  if (is_hit) {
    RTCHitN_geomID(hitn, args->N, 0) = 0;

    // hit normal
    RTCHitN_Ng_x(hitn, args->N, 0) = info.hitNormal.x();
    RTCHitN_Ng_y(hitn, args->N, 0) = info.hitNormal.y();
    RTCHitN_Ng_z(hitn, args->N, 0) = info.hitNormal.z();

    // uv
    RTCHitN_u(hitn, args->N, 0) = 0;
    RTCHitN_v(hitn, args->N, 0) = 0;
  } else {
    RTCHitN_geomID(hitn, args->N, 0) = RTC_INVALID_GEOMETRY_ID;
  }
}

EmbreeIntersector::EmbreeIntersector() {
  device = rtcNewDevice(nullptr);
  if (!device) {
    fprintf(stderr, "error %d: cannot create rtcDevice\n",
            rtcGetDeviceError(nullptr));
  }

  rtcSetDeviceErrorFunction(device, RTCErrorFunction, nullptr);

  scene = rtcNewScene(device);
}

EmbreeIntersector::~EmbreeIntersector() {
  rtcReleaseDevice(device);
  rtcReleaseScene(scene);
}

bool EmbreeIntersector::initialize() const {
  for (const auto& prim : primitives) {
    const auto geometry = prim->getGeometry();
    const auto shape = geometry->getShape();

    RTCGeometry rtc_geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(rtc_geometry, 1);
    rtcSetGeometryUserData(rtc_geometry, geometry.get());
    rtcSetGeometryIntersectFunction(rtc_geometry, RTCUserGeometryIntersect);

    rtcCommitGeometry(rtc_geometry);
    rtcAttachGeometry(scene, rtc_geometry);
    rtcReleaseGeometry(rtc_geometry);
  }

  rtcCommitScene(scene);

  return true;
}

}  // namespace Prl2