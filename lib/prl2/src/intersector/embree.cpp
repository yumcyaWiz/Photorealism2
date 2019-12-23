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

}  // namespace Prl2