#include "render.h"

#include "camera/pinhole.h"
#include "intersector/linear.h"
#include "material/diffuse.h"
#include "shape/plane.h"
#include "shape/sphere.h"

void Render::initScene() {
  const auto shape = std::make_shared<Prl2::Plane>();
  const auto mat1 =
      std::make_shared<Prl2::Diffuse>(Prl2::RGB2Spectrum(Prl2::RGB(0.8)));
  const auto prim_trans = std::make_shared<Prl2::Transform>(
      Prl2::rotate(Prl2::Vec3(M_PI / 4, 0, 0)));
  const auto prim = std::make_shared<Prl2::Primitive>(shape, mat1, prim_trans);

  const auto shape2 = std::make_shared<Prl2::Sphere>(10000.0f);
  const auto mat2 = std::make_shared<Prl2::Diffuse>(Prl2::SPD(0.8));
  const auto prim2_trans = std::make_shared<Prl2::Transform>(
      Prl2::translate(Prl2::Vec3(0, -10001, 0)));
  const auto prim2 =
      std::make_shared<Prl2::Primitive>(shape2, mat2, prim2_trans);

  std::shared_ptr<Prl2::LinearIntersector> intersector =
      std::make_shared<Prl2::LinearIntersector>();
  intersector->addPrimitive(prim);
  intersector->addPrimitive(prim2);

  renderer.scene.intersector = intersector;
}

std::thread Render::render() {
  std::thread rendering_thread([&] {
    while (true) {
      if (refreshRender) {
        const auto start_time = std::chrono::system_clock::now();
        renderer.render(cancelRender);

        if (cancelRender) {
          cancelRender = false;
        } else {
          std::cout << "Rendering Finished in "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::system_clock::now() - start_time)
                           .count()
                    << "ms" << std::endl;
          refreshRender = false;
        }
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
  return rendering_thread;
}