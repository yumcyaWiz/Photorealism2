#include "intersector/embree.h"

namespace Prl2 {

EmbreeIntersector::EmbreeIntersector() {
  device = rtcNewDevice(nullptr);
  if (!device) {
    fprintf(stderr, "error %d: cannot create rtcDevice\n",
            rtcGetDeviceError(nullptr));
  }

  scene = rtcNewScene(device);
}

bool EmbreeIntersector::initialize() const {
  for (const auto& p : primitives) {
    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(geometry, 1);
  }
}

}  // namespace Prl2