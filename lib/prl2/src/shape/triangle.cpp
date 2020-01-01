#include "shape/triangle.h"

namespace Prl2 {

Triangle::Triangle(const TriangleMesh* _mesh, unsigned int _face_index)
    : mesh(_mesh), face_index(_face_index) {}

bool Triangle::intersect(const Ray& ray, IntersectInfo& info) const {
  return false;
}

bool Triangle::occluded(const Ray& ray) const { return false; }

Bounds3 Triangle::getBounds() const { return Bounds3(); }

void Triangle::samplePoint(Sampler& sampler, Vec3& p, Vec3& n,
                           Real& pdf_area) const {}

}  // namespace Prl2
