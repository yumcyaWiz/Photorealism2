#include "io/io.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "stb_image_write.h"
#include "tinyexr.h"

namespace Prl2 {

void writePPM(const std::string& filename, int width, int height,
              const std::vector<float>& rgb) {
  std::ofstream file(filename);

  file << "P3" << std::endl;
  file << width << " " << height << std::endl;
  file << "255" << std::endl;

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int r = std::clamp(
          static_cast<int>(255 * rgb[3 * i + 3 * width * j + 0]), 0, 255);
      const int g = std::clamp(
          static_cast<int>(255 * rgb[3 * i + 3 * width * j + 1]), 0, 255);
      const int b = std::clamp(
          static_cast<int>(255 * rgb[3 * i + 3 * width * j + 2]), 0, 255);
      file << r << " " << g << " " << b << std::endl;
    }
  }

  std::cout << filename << " has been written out" << std::endl;

  file.close();
}

void writePNG(const std::string& filename, int width, int height,
              const std::vector<float>& rgb) {
  if (!stbi_write_png(filename.c_str(), width, height, 3, rgb.data(),
                      sizeof(float) * width)) {
    std::cerr << "failed to save png file" << std::endl;
  } else {
    std::cout << "saved png file [" << filename << "]" << std::endl;
  }
}

// https://github.com/syoyo/tinyexr
void writeEXR(const std::string& filename, int width, int height,
              const std::vector<float>& rgb) {
  EXRHeader header;
  InitEXRHeader(&header);

  EXRImage image;
  InitEXRImage(&image);

  image.num_channels = 3;

  std::vector<float> images[3];
  images[0].resize(width * height);
  images[1].resize(width * height);
  images[2].resize(width * height);

  for (int i = 0; i < width * height; ++i) {
    images[0][i] = rgb[3 * i + 0];
    images[1][i] = rgb[3 * i + 1];
    images[2][i] = rgb[3 * i + 2];
  }

  float* image_ptr[3];
  image_ptr[0] = &(images[2].at(0));
  image_ptr[1] = &(images[1].at(0));
  image_ptr[2] = &(images[0].at(0));

  image.images = (unsigned char**)image_ptr;
  image.width = width;
  image.height = height;

  header.num_channels = 3;
  header.channels = new EXRChannelInfo[header.num_channels];
  strncpy(header.channels[0].name, "B", 255);
  header.channels[0].name[strlen("B")] = '\0';
  strncpy(header.channels[1].name, "G", 255);
  header.channels[1].name[strlen("G")] = '\0';
  strncpy(header.channels[2].name, "R", 255);
  header.channels[2].name[strlen("R")] = '\0';

  header.pixel_types = new int[header.num_channels];
  header.requested_pixel_types = new int[header.num_channels];
  for (int i = 0; i < header.num_channels; ++i) {
    header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
    header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF;
  }

  const char* err = nullptr;
  int ret = SaveEXRImageToFile(&image, &header, filename.c_str(), &err);
  if (ret != TINYEXR_SUCCESS) {
    fprintf(stderr, "Save EXR error: %s\n", err);
    FreeEXRErrorMessage(err);
    return;
  }
  printf("Saved EXR file. [%s] \n", filename.c_str());

  delete[] header.channels;
  delete[] header.pixel_types;
  delete[] header.requested_pixel_types;
}

}  // namespace Prl2