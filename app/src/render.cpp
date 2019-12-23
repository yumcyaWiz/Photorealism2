#include "render.h"

#include "camera/pinhole.h"
#include "intersector/embree.h"
#include "intersector/linear.h"
#include "light/area-light.h"
#include "material/diffuse.h"
#include "material/glass.h"
#include "material/mirror.h"
#include "shape/plane.h"
#include "shape/sphere.h"

void Render::initScene() {
  const auto plane = std::make_shared<Prl2::Plane>();
  const auto sphere = std::make_shared<Prl2::Sphere>();

  const auto diffuse_white =
      std::make_shared<Prl2::Diffuse>(Prl2::RGB2Spectrum(Prl2::RGB(0.8)));
  const auto diffuse_green = std::make_shared<Prl2::Diffuse>(
      Prl2::RGB2Spectrum(Prl2::RGB(0.0, 0.8, 0.0)));
  const auto diffuse_red = std::make_shared<Prl2::Diffuse>(
      Prl2::RGB2Spectrum(Prl2::RGB(0.8, 0.0, 0.0)));
  const auto mirror =
      std::make_shared<Prl2::Mirror>(Prl2::RGB2Spectrum(Prl2::RGB(0.8)));
  const auto glass = std::make_shared<Prl2::Glass>(
      Prl2::SellmeierEquation(1.26, 0.15, 0.88, 0.009, 0.044, 106.82),
      Prl2::RGB2Spectrum(Prl2::RGB(0.8)));

  const auto geom1_trans =
      std::make_shared<Prl2::Transform>(Prl2::translate(Prl2::Vec3(0, 1, 0)));
  const auto geom1 = std::make_shared<Prl2::Geometry>(sphere, geom1_trans);

  const auto geom2_trans =
      std::make_shared<Prl2::Transform>(Prl2::scale(Prl2::Vec3(4)));
  const auto geom2 = std::make_shared<Prl2::Geometry>(plane, geom2_trans);

  const auto geom3_trans = std::make_shared<Prl2::Transform>(
      Prl2::translate(Prl2::Vec3(0, 4, 0)) * Prl2::scale(Prl2::Vec3(4)));
  const auto geom3 = std::make_shared<Prl2::Geometry>(plane, geom3_trans);

  const auto geom4_trans = std::make_shared<Prl2::Transform>(
      Prl2::translate(Prl2::Vec3(2, 2, 0)) * Prl2::scale(Prl2::Vec3(4)) *
      Prl2::rotateZ(-Prl2::PI_DIV_2));
  const auto geom4 = std::make_shared<Prl2::Geometry>(plane, geom4_trans);

  const auto geom5_trans = std::make_shared<Prl2::Transform>(
      Prl2::translate(Prl2::Vec3(-2, 2, 0)) * Prl2::scale(Prl2::Vec3(4)) *
      Prl2::rotateZ(Prl2::PI_DIV_2));
  const auto geom5 = std::make_shared<Prl2::Geometry>(plane, geom5_trans);

  const auto geom6_trans = std::make_shared<Prl2::Transform>(
      Prl2::translate(Prl2::Vec3(0, 2, -2)) * Prl2::scale(Prl2::Vec3(4)) *
      Prl2::rotateX(-Prl2::PI_DIV_2));
  const auto geom6 = std::make_shared<Prl2::Geometry>(plane, geom6_trans);

  const auto geom7_trans = std::make_shared<Prl2::Transform>(
      Prl2::translate(Prl2::Vec3(0, 3.9, 0)) * Prl2::scale(Prl2::Vec3(2)));
  const auto geom7 = std::make_shared<Prl2::Geometry>(plane, geom7_trans);

  const Prl2::SPD light_spd({400, 500, 600, 700}, {0, 8, 15.6, 18.4});
  const auto light = std::make_shared<Prl2::AreaLight>(0.01 * light_spd, geom7);

  const auto prim1 = std::make_shared<Prl2::Primitive>(geom1, glass);
  const auto prim2 = std::make_shared<Prl2::Primitive>(geom2, diffuse_white);
  const auto prim3 = std::make_shared<Prl2::Primitive>(geom3, diffuse_white);
  const auto prim4 = std::make_shared<Prl2::Primitive>(geom4, diffuse_green);
  const auto prim5 = std::make_shared<Prl2::Primitive>(geom5, diffuse_red);
  const auto prim6 = std::make_shared<Prl2::Primitive>(geom6, diffuse_white);
  const auto prim7 =
      std::make_shared<Prl2::Primitive>(geom7, diffuse_white, light);

  std::shared_ptr<Prl2::LinearIntersector> intersector =
      std::make_shared<Prl2::LinearIntersector>();
  intersector->addPrimitive(prim1);
  intersector->addPrimitive(prim2);
  intersector->addPrimitive(prim3);
  intersector->addPrimitive(prim4);
  intersector->addPrimitive(prim5);
  intersector->addPrimitive(prim6);
  intersector->addPrimitive(prim7);

  renderer.scene.intersector = intersector;
}

std::thread Render::render() {
  std::thread rendering_thread([&] {
    while (true) {
      if (refresh_render) {
        renderer.render(cancel_render);

        if (cancel_render) {
          cancel_render = false;
        } else {
          refresh_render = false;
        }
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
  });
  return rendering_thread;
}