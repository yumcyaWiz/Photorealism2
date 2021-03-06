#ifndef _PRL2_EMBREE_H
#define _PRL2_EMBREE_H
#include "intersector/intersector.h"

#include "embree3/rtcore.h"

namespace Prl2 {

class EmbreeIntersector : public Intersector {
 public:
  EmbreeIntersector();
  ~EmbreeIntersector();

  virtual bool initialize() override;
  virtual bool intersect(const Ray& ray, IntersectInfo& info) const override;

 private:
  RTCDevice device;
  RTCScene scene;
};

}  // namespace Prl2

#endif