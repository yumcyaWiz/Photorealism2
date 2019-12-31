#ifndef _PRL2_TRIANGLE_H
#define _PRL2_TRIANGLE_H

#include "core/type.h"

namespace Prl2 {

// 三角形メッシュを表す
struct TriangleMesh {
  unsigned int num_vertices;  // 頂点数
  unsigned int num_faces;     // 面の数
  Real* vertices;             // 頂点配列
  Real* indices;              // 頂点インデックス配列
  Real* normals;              // 頂点法線
  Real* uvs;                  // 頂点UV
};

class Triangle {
 public:
  Triangle(TriangleMesh* _mesh, unsigned int _face_index);

 private:
  const TriangleMesh* mesh;       // TriangleMesh
  const unsigned int face_index;  // TriangleMesh中の面インデックス
};

}  // namespace Prl2
#endif