#include <iostream>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "transform.h"
#include "ray.h"
#include "film.h"


int main() {
  Vec3 p(1, 0, 0);
  Transform t = rotateZ(2*M_PI);
  
  std::cout << t.applyPoint(p) << std::endl;

  return 0;
}
