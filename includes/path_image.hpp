#ifndef PATH_IMAGE_H
#define PATH_IMAGE_H

#include <cstdlib>  // for size_t
#include <string>
#include <vector>

#include "color.hpp"
#include "grayscale_image.hpp"
#include "path.hpp"

class PathImage {
public:
  // write behavior declarations here; define in path_image.cc.
  PathImage(const GrayscaleImage& image, const ElevationDataset& dataset);
  size_t Width() const;
  size_t Height() const;
  unsigned int MaxColorValue() const;
  const std::vector<Path>& Paths() const;
  const std::vector<std::vector<Color>>& GetPathImage() const;
  void ToPpm(const std::string& name) const;

  // my own helper functions
  // void Copy(const GrayscaleImage& image, size_t width_, size_t height_);
  void PopulatePaths(size_t width_,
                     size_t height_,
                     const ElevationDataset& dataset,
                     Path& best_path_row);
  void Coloring(std::vector<Path> paths_, Path const& best_path_row);

private:
  // vector of Paths of traversal for each image_ row
  std::vector<Path> paths_;
  // og image overlaid with paths
  std::vector<std::vector<Color>> path_image_;

  size_t width_ = 0;
  size_t height_ = 0;
  static const int kMaxColorValue = 255;
  const Color kRed = Color(252, 25, 63);
  const Color kGreen = Color(31, 253, 13);
};

#endif