#ifndef FILM_H
#define FILM_H

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include "core/spectrum.h"
#include "core/type.h"
#include "core/vec2.h"

namespace Prl2 {

//フィルム面を表すクラス
// SPDを画素ごとに格納する
//フィルム面の位置は[-0.5*width_length, 0.5*width_length] x [-0.5*height_length,
// 0.5*height_length]で表される
class Film {
 public:
  unsigned int width;    //横幅
  unsigned int height;   //縦幅
  Real width_length;     //横の物理的長さ[m]
  Real height_length;    //縦の物理的長さ[m]
  Real diagonal_length;  //対角線の物理的長さ[m]
  std::vector<SPD> pixels;

  Film(unsigned int _width, unsigned int _height,
       const Real& _width_length = 0.0251f,
       const Real& _height_length = 0.0251f)
      : width(_width),
        height(_height),
        width_length(_width_length),
        height_length(_height_length),
        diagonal_length(std::sqrt(width_length * width_length +
                                  height_length * height_length)) {
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

  //(u, v)の物理的な位置を計算
  Vec2 computePosition(const Real& u, const Real& v) const {
    return Vec2(u * 0.5f * width_length, v * 0.5f * height_length);
  };

  //(i, j)をある値で割る
  void divide(int i, int j, const Real& k) {
    assert(i >= 0 && i < static_cast<int>(width));
    assert(j >= 0 && j < static_cast<int>(height));
    pixels[i + width * j] /= k;
  };
  // フィルム全体をある値で割る
  void divide(const Real& k) {
    for (unsigned int j = 0; j < height; ++j) {
      for (unsigned int i = 0; i < width; ++i) {
        divide(i, j, k);
      }
    }
  };

  // フィルム全体をクリア
  void clear() {
    for (unsigned int j = 0; j < height; ++j) {
      for (unsigned int i = 0; i < width; ++i) {
        pixels[i + width * j].clear();
      }
    }
  };

  // 指定したピクセルをクリア
  void clearPixel(int i, int j) { pixels[i + width * j].clear(); }

  // フィルムをリサイズ
  void resize(unsigned int _width, unsigned int _height) {
    width = _width;
    height = _height;
    pixels.resize(_width * _height);
  };

  // フィルムの物理的長さをリサイズ
  void resizeLength(const Real& _width_length, const Real& _height_length) {
    width_length = _width_length;
    height_length = _height_length;
    diagonal_length =
        std::sqrt(width_length * width_length + height_length * height_length);
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