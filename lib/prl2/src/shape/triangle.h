#ifndef _PRL2_TRIANGLE_H
#define _PRL2_TRIANGLE_H

#include <memory>

#include "core/type.h"
#include "shape/shape.h"

namespace Prl2 {

// 三角形メッシュを表す
struct TriangleMesh {
  unsigned int num_vertices;  // 頂点数
  unsigned int num_faces;     // 面の数
  Vec3* vertices;             // 頂点座標
  unsigned int* indices;      // 頂点インデックス配列
  Vec3* normals;              // 頂点法線
  Vec2* uvs;                  // 頂点UV

  TriangleMesh() {
    num_vertices = 0;
    num_faces = 0;
    vertices = nullptr;
    indices = nullptr;
    normals = nullptr;
    uvs = nullptr;
  };
};

class Triangle : public Shape {
 public:
  Triangle(const std::shared_ptr<TriangleMesh>& _mesh,
           unsigned int _face_index);

  bool intersect(const Ray& ray, IntersectInfo& info) const override;

  bool occluded(const Ray& ray) const override;

  Bounds3 getBounds() const override;

  void samplePoint(Sampler& sampler, Vec3& p, Vec3& n,
                   Real& pdf_area) const override;

 private:
  const std::shared_ptr<TriangleMesh> mesh;
  const unsigned int face_index;  // TriangleMesh中の面インデックス
};

}  // namespace Prl2
#endif