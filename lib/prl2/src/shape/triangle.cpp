#include "shape/triangle.h"

#include "sampler/sampling.h"

namespace Prl2 {

Triangle::Triangle(const std::shared_ptr<TriangleMesh>& _mesh,
                   unsigned int face_index)
    : mesh(_mesh),
      v0(_mesh->indices[3 * face_index]),
      v1(_mesh->indices[3 * face_index + 1]),
      v2(_mesh->indices[3 * face_index + 2]) {
  const Vec3& p0 = mesh->vertices[v0];
  const Vec3& p1 = mesh->vertices[v0];
  const Vec3& p2 = mesh->vertices[v0];
  face_area = 0.5f * length(cross(p1 - p0, p2 - p0));
}

// Möller–Trumbore intersection algorithm
// https://www.wikiwand.com/en/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::intersect(const Ray& ray, IntersectInfo& info) const {
  const Vec3& p0 = mesh->vertices[v0];
  const Vec3& p1 = mesh->vertices[v1];
  const Vec3& p2 = mesh->vertices[v2];

  const Vec3 edge1 = p1 - p0;
  const Vec3 edge2 = p2 - p0;
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

  // compute hit position
  info.hitPos = ray(t);

  // compute normal
  if (mesh->normals) {
    const Vec3& n0 = mesh->normals[v0];
    const Vec3& n1 = mesh->normals[v1];
    const Vec3& n2 = mesh->normals[v2];
    info.hitNormal = lerp3(u, v, n0, n1, n2);
  } else {
    info.hitNormal = normalize(cross(edge1, edge2));
  }

  // compute uv
  if (mesh->uvs) {
    const Vec2& uv0 = mesh->uvs[v0];
    const Vec2& uv1 = mesh->uvs[v1];
    const Vec2& uv2 = mesh->uvs[v2];
    info.uv = lerp3(u, v, uv0, uv1, uv2);
  } else {
    info.uv = Vec2(u, v);
  }

  return true;
}

bool Triangle::occluded(const Ray& ray) const {
  const Vec3& p0 = mesh->vertices[v0];
  const Vec3& p1 = mesh->vertices[v1];
  const Vec3& p2 = mesh->vertices[v2];

  const Vec3 edge1 = p1 - p0;
  const Vec3 edge2 = p2 - p0;
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

  return true;
}

Bounds3 Triangle::getBounds() const {
  const Vec3& p0 = mesh->vertices[v0];
  const Vec3& p1 = mesh->vertices[v1];
  const Vec3& p2 = mesh->vertices[v2];

  const Real p0x = std::min(p0.x(), std::min(p1.x(), p2.x()));
  const Real p0y = std::min(p0.y(), std::min(p1.y(), p2.y()));
  const Real p0z = std::min(p0.z(), std::min(p1.z(), p2.z()));

  const Real p1x = std::max(p0.x(), std::max(p1.x(), p2.x()));
  const Real p1y = std::max(p0.y(), std::max(p1.y(), p2.y()));
  const Real p1z = std::max(p0.z(), std::max(p1.z(), p2.z()));

  return Bounds3(Vec3(p0x, p0y, p0z), Vec3(p1x, p1y, p1z));
}

void Triangle::samplePoint(Sampler& sampler, Vec3& p, Vec3& n,
                           Real& pdf_area) const {
  const Vec3& p0 = mesh->vertices[v0];
  const Vec3& p1 = mesh->vertices[v1];
  const Vec3& p2 = mesh->vertices[v2];

  const Vec2 uv = sampleTriangle(sampler.getNext2D());

  p = lerp3(uv.x(), uv.y(), p0, p1, p2);

  if (mesh->normals) {
    const Vec3& n0 = mesh->normals[v0];
    const Vec3& n1 = mesh->normals[v1];
    const Vec3& n2 = mesh->normals[v2];
    n = lerp3(uv.x(), uv.y(), n0, n1, n2);
  } else {
    n = normalize(cross(p1 - p0, p2 - p0));
  }

  pdf_area = 1 / face_area;
}

}  // namespace Prl2
