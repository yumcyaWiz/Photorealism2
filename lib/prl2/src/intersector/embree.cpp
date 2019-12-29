#include "intersector/embree.h"

namespace Prl2 {

void RTCErrorFunction(void* userPtr, RTCError code, const char* str) {
  fprintf(stderr, "error %d: %s", code, str);
}

void RTCUserGeometryIntersect(const RTCIntersectFunctionNArguments* args) {
  const Primitive* prim =
      reinterpret_cast<const Primitive*>(args->geometryUserPtr);

  assert(args->N == 1);
  if (!args->valid[0]) {
    return;
  }

  RTCRayN* rayn = RTCRayHitN_RayN(args->rayhit, args->N);
  RTCHitN* hitn = RTCRayHitN_HitN(args->rayhit, args->N);

  // compute ray
  const float ox = RTCRayN_org_x(rayn, args->N, 0);
  const float oy = RTCRayN_org_y(rayn, args->N, 0);
  const float oz = RTCRayN_org_z(rayn, args->N, 0);
  const float dx = RTCRayN_dir_x(rayn, args->N, 0);
  const float dy = RTCRayN_dir_y(rayn, args->N, 0);
  const float dz = RTCRayN_dir_z(rayn, args->N, 0);
  const Ray ray(Vec3(ox, oy, oz), Vec3(dx, dy, dz));

  // intersect
  IntersectInfo info;
  bool is_hit = prim->intersect(ray, info);

  // set intersect info
  if (is_hit) {
    RTCRayN_tfar(rayn, args->N, 0) = info.t;  // hit distance

    // hit normal
    RTCHitN_Ng_x(hitn, args->N, 0) = info.hitNormal.x();
    RTCHitN_Ng_y(hitn, args->N, 0) = info.hitNormal.y();
    RTCHitN_Ng_z(hitn, args->N, 0) = info.hitNormal.z();

    // uv
    RTCHitN_u(hitn, args->N, 0) = 0;
    RTCHitN_v(hitn, args->N, 0) = 0;

    // geom_id and prim_id
    RTCHitN_geomID(hitn, args->N, 0) = prim->getID();
    RTCHitN_primID(hitn, args->N, 0) = args->primID;
  } else {
    RTCHitN_geomID(hitn, args->N, 0) = RTC_INVALID_GEOMETRY_ID;
  }
}

void RTCUserGeometryBound(const RTCBoundsFunctionArguments* args) {
  const Primitive* prim =
      reinterpret_cast<const Primitive*>(args->geometryUserPtr);

  // compute bounds
  const Bounds3 bounds = prim->getBounds();

  // set bounds
  RTCBounds* bounds_o = args->bounds_o;
  bounds_o->lower_x = bounds.p1.x();
  bounds_o->lower_y = bounds.p1.y();
  bounds_o->lower_z = bounds.p1.z();
  bounds_o->upper_x = bounds.p2.x();
  bounds_o->upper_y = bounds.p2.y();
  bounds_o->upper_z = bounds.p2.z();
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

bool EmbreeIntersector::initialize() {
  unsigned int geom_id = 0;
  for (const auto& prim : primitives) {
    // create user defined geometry
    RTCGeometry rtc_geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(rtc_geometry, 1);
    rtcSetGeometryUserData(rtc_geometry, prim.get());
    rtcSetGeometryIntersectFunction(rtc_geometry, RTCUserGeometryIntersect);
    rtcSetGeometryBoundsFunction(rtc_geometry, RTCUserGeometryBound, nullptr);

    rtcCommitGeometry(rtc_geometry);

    // set geometry id
    rtcAttachGeometry(scene, rtc_geometry);
    prim->setID(geom_id);

    rtcReleaseGeometry(rtc_geometry);
    geom_id++;
  }

  rtcCommitScene(scene);

  return true;
}

bool EmbreeIntersector::intersect(const Ray& ray, IntersectInfo& info) const {
  // init ray hit
  RTCRayHit rayhit;
  rayhit.ray.org_x = ray.origin.x();
  rayhit.ray.org_y = ray.origin.y();
  rayhit.ray.org_z = ray.origin.z();
  rayhit.ray.dir_x = ray.direction.x();
  rayhit.ray.dir_y = ray.direction.y();
  rayhit.ray.dir_z = ray.direction.z();
  rayhit.ray.tnear = ray.tmin;
  rayhit.ray.tfar = ray.tmax;
  rayhit.ray.mask = 0;
  rayhit.ray.flags = 0;
  rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

  // intersect
  RTCIntersectContext context;
  rtcIntersect1(scene, &context, &rayhit);

  if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
    info.t = rayhit.ray.tfar;
    info.hitPos = ray(info.t);
    info.hitNormal = Vec3(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);
    info.hitPrimitive = primitives[rayhit.hit.geomID].get();
    return true;
  } else {
    return false;
  }
}

}  // namespace Prl2