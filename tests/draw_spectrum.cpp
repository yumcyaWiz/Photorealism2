#include "camera/film.h"
#include "core/spectrum.h"

using namespace Prl2;

int main() {
  Prl2::Film film(512, 50);
  for (std::size_t i = 0; i < film.width; ++i) {
    for (std::size_t j = 0; j < film.height; ++j) {
      const Prl2::Real l = float(i) / film.width * (780 - 380) + 380;
      film.addPixel(i, j, l, 1);
    }
  }
  film.writePPM("output.ppm");
}