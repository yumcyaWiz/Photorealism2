#ifndef FILM_H
#define FILM_H

#include <vector>

#include "vec3.h"
#include "spectrum.h"


class Film {
  public:
  std::size_t width;
  std::size_t height;
  std::vector<Spectrum> pixels;

  Film(const std::size_t& _width, const std::size_t& _height) : width(_width), height(_height) {
    pixels.resize(width * height);
  };
};


#endif