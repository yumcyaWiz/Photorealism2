#include "shape/triangle.h"

namespace Prl2 {

Triangle::Triangle(const std::shared_ptr<TriangleMesh>& _mesh,
                   unsigned int _face_index)
    : mesh(_mesh), face_index(_face_index) {}

// Möller–Trumbore intersection algorithm
// https://www.wikiwand.com/en/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::intersect(const Ray& ray, IntersectInfo& info) const {
  const unsigned int f0 = mesh->indices[3 * face_index];
  const unsigned int f1 = mesh->indices[3 * face_index + 1];
  const unsigned int f2 = mesh->indices[3 * face_index + 2];

  const Vec3& v0 = mesh->vertices[f0];
  const Vec3& v1 = mesh->vertices[f1];
  const Vec3& v2 = mesh->vertices[f2];

  const Vec3 edge1 = v1 - v0;
  const Vec3 edge2 = v2 - v0;
  const Vec3 h = cross(ray.direction, edge2);

  const Real a = dot(edge1, h);
  if (a > -EPS && a < EPS) {
    return false;
  }

  const Real f = 1.0f / a;
  const Vec3 s = ray.origin - v0;
  const Real u = f * dot(s, h);
  if (u < 0.0f || u > 1.0f) {
    return false;
  }

  const Vec3 q = cross(s, edge1);
  const Real v = f * dot(ray.direction, q);

  if (v < 0.0f || u + v > 1.0f) {
    return false;
  }

  // compute hit distance
  const Real t = f * dot(edge2, q);
  if (t < ray.tmin || t > ray.tmax) {
    return false;
  }

  info.t = t;
  info.hitPos = ray(t);

  // compute normal
  if (mesh->normals) {
    const Vec3& n0 = mesh->normals[f0];
    const Vec3& n1 = mesh->normals[f1];
    const Vec3& n2 = mesh->normals[f2];
  }

  return false;
}

bool Triangle::occluded(const Ray& ray) const { return false; }

Bounds3 Triangle::getBounds() const { return Bounds3(); }

void Triangle::samplePoint(Sampler& sampler, Vec3& p, Vec3& n,
                           Real& pdf_area) const {}

}  // namespace Prl2
