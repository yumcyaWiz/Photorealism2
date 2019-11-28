#ifndef _PRL2_IBL_SKY_H
#define _PRL2_IBL_SKY_H

#include <string>

#include "sky/sky.h"

namespace Prl2 {
class IBLSky : public Sky {
 public:
  IBLSky(const std::string& filename);

  Real getRadiance(const Ray& ray) const override;
};

}  // namespace Prl2

#endif