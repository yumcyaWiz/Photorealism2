#ifndef FILM_H
#define FILM_H

#include <fstream>
#include <iostream>
#include <vector>

#include "spectrum.h"
#include "vec3.h"

class Film {
 public:
  std::size_t width;
  std::size_t height;
  std::vector<SPD> pixels;

  Film(const std::size_t& _width, const std::size_t& _height)
      : width(_width), height(_height) {
    pixels.resize(width * height);
  };

  // (i, j)のSPDを入手
  SPD getPixel(const std::size_t& i, const std::size_t& j) const {
    return pixels[i + width * j];
  };

  //(i, j)にSPDをセット
  void setPixel(const std::size_t& i, const std::size_t& j, const SPD& spd) {
    pixels[i + width * j] = spd;
  };

  // PPMを出力
  void writePPM(const std::string& filename) const;
};

#endif