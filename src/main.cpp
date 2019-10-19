#include <iostream>
#include <thread>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "transform.h"
#include "ray.h"
#include "film.h"
#include "gui.h"


int main() {
  GUI gui;

  std::thread renderThread(gui.render);

  renderThread.join();

  return 0;
}
