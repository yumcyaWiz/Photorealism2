#include "render.h"

#include "camera/pinhole.h"
#include "intersector/linear.h"
#include "material/diffuse.h"
#include "material/mirror.h"
#include "shape/plane.h"
#include "shape/sphere.h"

void Render::initScene() {
  const auto plane = std::make_shared<Prl2::Plane>();
  const auto sphere = std::make_shared<Prl2::Sphere>();
  const auto diffuse_white =
      std::make_shared<Prl2::Diffuse>(Prl2::RGB2Spectrum(Prl2::RGB(0.8)));
  const auto mirror =
      std::make_shared<Prl2::Mirror>(Prl2::RGB2Spectrum(Prl2::RGB(0.8)));
  const auto prim1_trans =
      std::make_shared<Prl2::Transform>(Prl2::translate(Prl2::Vec3(0, 1, 0)));
  const auto prim1 =
      std::make_shared<Prl2::Primitive>(sphere, diffuse_white, prim1_trans);

  const auto prim2_trans =
      std::make_shared<Prl2::Transform>(Prl2::scale(Prl2::Vec3(4)));
  const auto prim2 =
      std::make_shared<Prl2::Primitive>(plane, mirror, prim2_trans);

  std::shared_ptr<Prl2::LinearIntersector> intersector =
      std::make_shared<Prl2::LinearIntersector>();
  intersector->addPrimitive(prim1);
  intersector->addPrimitive(prim2);

  renderer.scene.intersector = intersector;
}

std::thread Render::render() {
  std::thread rendering_thread([&] {
    while (true) {
      if (refresh_render) {
        const auto start_time = std::chrono::system_clock::now();
        renderer.render(cancel_render);

        if (cancel_render) {
          cancel_render = false;
        } else {
          std::cout << "Rendering Finished in "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::system_clock::now() - start_time)
                           .count()
                    << "ms" << std::endl;
          refresh_render = false;
        }
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
  return rendering_thread;
}