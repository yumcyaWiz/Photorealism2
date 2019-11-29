#ifndef _PRL2_IO_H
#define _PRL2_IO_H

#include <string>
#include <vector>

namespace Prl2 {

// RGBの配列からPPM画像を保存する
void writePPM(const std::string& filename, int width, int height,
              const std::vector<float>& rgb);

// RGBの配列からPNG画像を保存する
void writePNG(const std::string& filename, int width, int height,
              const std::vector<float>& rgb);

// RGBの配列からEXR画像を保存する
void writeEXR(const std::string& filename, int width, int height,
              const std::vector<float>& rgb);

}  // namespace Prl2

#endif