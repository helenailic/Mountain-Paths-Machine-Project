#include "grayscale_image.hpp"

#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>

// construct grayscale image from elevation data
// high elevation = high brightness
// low elevation = low brightness

// to represent color, make Color object
// for grayscale image, construct GrayscaleImage object
// encodes topological map by housing std::vector<std::vector<Color>>
// encodes topological map in 2d vector of colors

GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset) {
  width_ = dataset.Width();
  height_ = dataset.Height();
  int shade_of_gray = 0;
  // to be gray, R G B all same value b/w 0-255
  // shade_of_gray = std::round((elevation_point - min_ele) / (max_ele -
  // min_ele) * 255) pay attention to types being divided otherwise info lost if
  // denom = 0 (ie max_ele == min_ele) shade_of_gray = 0

  // populates 2d vector of Colors with color rep of each elevation datapoint
  // image_
  // iterate through elevation dataset and make shades_of_gray
  for (unsigned int i = 0; i < height_; ++i) {
    std::vector<Color> temp_row;
    for (unsigned int j = 0; j < width_; ++j) {
      // does this correctly pay attention to types being divided so that I
      // don't wrongly truncate?
      if (dataset.MaxEle() == dataset.MinEle()) {
        shade_of_gray = 0;
      } else {
        shade_of_gray =
            std::round((double)(dataset.DatumAt(i, j) - dataset.MinEle()) /
                       (double)(dataset.MaxEle() - dataset.MinEle()) *
                       kMaxColorValue);  // if this stops working cast as double
      }
      temp_row.push_back(Color(shade_of_gray, shade_of_gray, shade_of_gray));
    }
    image_.push_back(temp_row);
  }
}

GrayscaleImage::GrayscaleImage(const std::string& filename,
                               size_t width,
                               size_t height) {
  ElevationDataset dataset = ElevationDataset(filename, width, height);
  width_ = dataset.Width();
  height_ = dataset.Height();
  int shade_of_gray = 0;
  for (unsigned int i = 0; i < height_; ++i) {
    std::vector<Color> temp_row;
    for (unsigned int j = 0; j < width_; ++j) {
      // does this correctly pay attention to types being divided so that I
      // don't wrongly truncate?
      if (dataset.MaxEle() == dataset.MinEle()) {
        shade_of_gray = 0;
      } else {
        shade_of_gray = std::round(
            (double)(dataset.DatumAt(i, j) - dataset.MinEle()) /
            (double)(dataset.MaxEle() - dataset.MinEle()) * kMaxColorValue);
      }
      temp_row.push_back(Color(shade_of_gray, shade_of_gray, shade_of_gray));
    }
    image_.push_back(temp_row);
  }
}

size_t GrayscaleImage::Width() const { return width_; }
size_t GrayscaleImage::Height() const { return height_; }

unsigned int GrayscaleImage::MaxColorValue() const { return kMaxColorValue; }

const Color& GrayscaleImage::ColorAt(int row, int col) const {
  return image_.at(row).at(col);  // is this returning by reference?
}

const std::vector<std::vector<Color>>& GrayscaleImage::GetImage() const {
  return image_;  // is this returning by reference
}

// writes out image_ in plain PPM format    name is filename
void GrayscaleImage::ToPpm(const std::string& name) const {
  // text file interpreted by computer as image file
  std::ofstream ofs{name};
  if (!ofs.is_open()) {
    throw std::runtime_error("output file couldn't open");
  }
  ofs << "P3" << '\n';
  ofs << width_ << " " << height_ << '\n';
  ofs << kMaxColorValue << '\n';
  for (unsigned int i = 0; i < height_; ++i) {
    for (unsigned int j = 0; j < width_; ++j) {
      ofs << image_.at(i).at(j).Red() << " " << image_.at(i).at(j).Green()
          << " " << image_.at(i).at(j).Blue() << " ";
    }
    ofs << '\n';
  }
}