#include "renderer/scene-loader.h"

#include "tiny_obj_loader.h"

namespace Prl2 {

bool loadTriangleMeshFromObj(const std::string& filename) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string warn;
  std::string err;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              filename.c_str());

  if (!warn.empty()) {
    std::cout << warn << std::endl;
  }

  if (!err.empty()) {
    std::cout << err << std::endl;
  }

  if (!ret) {
    return false;
  }

  for (size_t s = 0; s < shapes.size(); ++s) {
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f) {
      const size_t fv = shapes[s].mesh.num_face_vertices[f];
      for (size_t v = 0; v < fv; ++v) {
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
      }
    }
  }

  return true;
}

}  // namespace Prl2