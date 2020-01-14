#ifndef _PRL2_TONE_MAPPING_H
#define _PRL2_TONE_MAPPING_H

#include <cmath>
#include <vector>

namespace Prl2 {

// Reinhard Tone Mapping
inline float reinhardToneMapping(float luminance, float exposure) {
  return luminance * (1 + luminance / (exposure * exposure)) / (1 + luminance);
}

}  // namespace Prl2

#endif