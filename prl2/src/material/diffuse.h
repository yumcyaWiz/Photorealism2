#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "core/spectrum.h"
#include "material/material.h"

namespace Prl2 {

class Diffuse : public Material {
 public:
  Diffuse(const SPD& _albedo) : albedo(_albedo){};

  Real sampleDirection(const Ray& ray, const IntersectInfo& info, Ray& ray_out,
                       Real& pdf) const {};

 private:
  SPD albedo;  //分光反射率
};

}  // namespace Prl2

#endif