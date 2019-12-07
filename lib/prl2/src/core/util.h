#ifndef _PRL2_UTIL_H
#define _PRL2_UTIL_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "core/constant.h"
#include "core/type.h"

namespace Prl2 {

// Degree to Radian
inline constexpr Real degToRad(const Real& deg) { return deg / 180 * PI }

// Radian to Degree
inline constexpr Real radToDeg(const Real& rad) { return rad * INV_PI }

}  // namespace Prl2

#endif