#include "shape/triangle.h"

namespace Prl2 {

Triangle::Triangle(const TriangleMesh* _mesh, unsigned int _face_index)
    : mesh(_mesh), face_index(_face_index) {}

}  // namespace Prl2
