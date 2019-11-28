#ifndef _PRL2_IBL_SKY_H
#define _PRL2_IBL_SKY_H

#include <string>

#include "core/spectrum.h"
#include "sky/sky.h"

namespace Prl2 {
class IBLSky : public Sky {
 public:
  IBLSky(const std::string& filename);
  ~IBLSky();

  Real getRadiance(const Ray& ray) const override;

 private:
  int width;      // 横幅[px]
  int height;     // 縦幅[px]
  float* pixels;  // 画素の配列(容量削減のためRGBで保存)
};

}  // namespace Prl2

#endif