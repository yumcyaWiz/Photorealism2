#include "intersector/embree.h"

namespace Prl2 {

void RTCErrorFunction(void* userPtr, RTCError code, const char* str) {
  fprintf(stderr, "error %d: %s", code, str);
}

void RTCUserGeometryIntersect(const RTCIntersectFunctionNArguments* args) {
  const Geometry* geometry =
      reinterpret_cast<const Geometry*>(args->geometryUserPtr);
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

    rtcCommitGeometry(rtc_geometry);
    rtcAttachGeometry(scene, rtc_geometry);
    rtcReleaseGeometry(rtc_geometry);
  }

  rtcCommitScene(scene);

  return true;
}

}  // namespace Prl2