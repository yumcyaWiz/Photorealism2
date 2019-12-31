#ifndef _PRL2_TRIANGLE_H
#define _PRL2_TRIANGLE_H

namespace Prl2 {

class Triangle {
 public:
  Triangle(unsigned int _face_index);

 private:
  const unsigned int face_index;  // TriangleMesh中の面インデックス
};

}  // namespace Prl2
#endif