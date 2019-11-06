#ifndef FILM_H
#define FILM_H

#include <cassert>
#include <string>
#include <vector>

#include "core/spectrum.h"
#include "core/vec2.h"

namespace Prl2 {

//フィルム面を表すクラス
// SPDを画素ごとに格納する
//フィルム面の位置は[-0.5*width_length, 0.5*width_length] x [-0.5*height_length,
// 0.5*height_length]で表される
class Film {
 public:
  unsigned int width;       //横幅
  unsigned int height;      //縦幅
  Real width_length;        //横の物理的長さ[m]
  Real height_length;       //縦の物理的長さ[m]
  std::vector<SPD> pixels;  //画素

  Film(const unsigned int _width, const unsigned int _height)
      : width(_width), height(_height) {
    pixels.resize(width * height);
  };

  // (i, j)のSPDを入手
  SPD getPixel(int i, int j) const {
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    return pixels[i + width * j];
  };
  //物理的位置からSPDを入手
  SPD getpixel(const Vec2& v) const {
    int i, j;
    computeIndex(v, i, j);
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
  //物理的位置にSPDをセット
  void setPixel(const Vec2& v, const SPD& spd) {
    int i, j;
    computeIndex(v, i, j);
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
  //物理的位置にSPDを加算
  void addPixel(const Vec2& v, const SPD& spd) {
    int i, j;
    computeIndex(v, i, j);
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    pixels[i + width * j] += spd;
  }

  //(i, j)に分光放射束を加算
  void addPixel(int i, int j, const Real& _lambda, const Real& _phi) {
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    pixels[i + width * j].addPhi(_lambda, _phi);
  };
  //物理的位置に分光放射束を加算
  void addPixel(const Vec2& v, const Real& _lambda, const Real& _phi) {
    int i, j;
    computeIndex(v, i, j);
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    pixels[i + width * j].addPhi(_lambda, _phi);
  };

  // PPMを出力
  void writePPM(const std::string& filename) const;

 private:
  //物理的位置からインデックスを計算
  void computeIndex(const Vec2& v, int& i, int& j) const {
    i = (v.x() + 0.5f * width_length) / width_length * width;
    j = (v.y() + 0.5f * height_length) / height_length * height;
  };
};

}  // namespace Prl2

#endif