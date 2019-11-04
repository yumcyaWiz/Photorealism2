#ifndef FILM_H
#define FILM_H

#include <cassert>
#include <string>
#include <vector>

#include "spectrum.h"

class Film {
 public:
  std::size_t width;        //横幅
  std::size_t height;       //縦幅
  std::vector<SPD> pixels;  //画素

  Film(const std::size_t& _width, const std::size_t& _height);

  // (i, j)のSPDを入手
  SPD getPixel(int i, int j) const;

  //(i, j)にSPDをセット
  void setPixel(int i, int j, const SPD& spd);

  //(i, j)にSPDを加算
  void addPixel(int i, int j, const SPD& spd);

  // PPMを出力
  void writePPM(const std::string& filename) const;
};

#endif