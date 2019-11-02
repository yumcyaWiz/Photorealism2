#include <iostream>
#include <thread>

#include "film.h"
#include "gui.h"
#include "mat4.h"
#include "ray.h"
#include "transform.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

int main() {
  GUI gui;

  std::thread renderThread(gui.render);

  renderThread.join();

  return 0;
}
