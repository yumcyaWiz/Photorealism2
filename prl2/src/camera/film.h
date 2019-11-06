#ifndef FILM_H
#define FILM_H

#include <cassert>
#include <string>
#include <vector>

#include "core/spectrum.h"

namespace Prl2 {

class Film {
 public:
  std::size_t width;        //横幅
  std::size_t height;       //縦幅
  std::vector<SPD> pixels;  //画素

  Film(const std::size_t& _width, const std::size_t& _height)
      : width(_width), height(_height) {
    pixels.resize(width * height);
  };

  // (i, j)のSPDを入手
  SPD getPixel(int i, int j) const {
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    return pixels[i + width * j];
  };

  //(i, j)にSPDをセット
  void setPixel(int i, int j, const SPD& spd) {
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    pixels[i + width * j] = spd;
  };

  //(i, j)にSPDを加算
  void addPixel(int i, int j, const SPD& spd) {
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    pixels[i + width * j] += spd;
  };

  //(i, j)に分光放射束を加算
  void addPixel(int i, int j, const Real& _lambda, const Real& _phi) {
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    pixels[i + width * j].addPhi(_lambda, _phi);
  };

  // PPMを出力
  void writePPM(const std::string& filename) const;
};

}  // namespace Prl2

#endif