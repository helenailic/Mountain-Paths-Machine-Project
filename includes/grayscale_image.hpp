#ifndef GRAYSCALE_IMAGE_H
#define GRAYSCALE_IMAGE_H

#include <cstdlib>  // for size_t
#include <vector>

#include "color.hpp"
#include "elevation_dataset.hpp"

class GrayscaleImage {
public:
  // write behavior declarations here; define in grayscale_image.cc.
  GrayscaleImage(const ElevationDataset& dataset);
  GrayscaleImage(const std::string& filename, size_t width, size_t height);
  size_t Width() const;
  size_t Height() const;
  unsigned int MaxColorValue() const;
  const Color& ColorAt(int row, int col) const;
  const std::vector<std::vector<Color>>& GetImage() const;
  void ToPpm(const std::string& name) const;

private:
  size_t width_;
  size_t height_;

  // 2d vector of colors (ie elevation data)
  std::vector<std::vector<Color>> image_;

  // max possible grayscale value of image
  static const int kMaxColorValue = 255;
};

#endif