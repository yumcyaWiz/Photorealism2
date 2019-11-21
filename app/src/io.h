#ifndef IO_H
#define IO_H

#include <string>
#include <vector>

// RGBの配列からPPM画像を保存する
void savePPM(const std::string& filename, int width, int height,
             const std::vector<float>& rgb);

#endif