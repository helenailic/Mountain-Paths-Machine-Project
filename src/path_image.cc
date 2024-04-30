#include "path_image.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

// find path of least elevation change
// PathImage object will be initialized with the topological image map,
// elevation data, width, height of dataset

// starting from each datapt in 0th col, least-elevation-change walk calculated
// until last col path found for each row encoded as Path object, stored in
// PathImage object's paths_ collection result: height number of paths stored in
// PathImage object
// image_ encodes topological image map
// path_image_ encodes the red and green paths

void PathImage::PopulatePaths(size_t width_,
                              size_t height_,
                              const ElevationDataset& dataset,
                              Path& best_path_row) {
  for (unsigned int i = 0; i < height_; ++i) {
    Path path(width_, i);
    path.SetLoc(0, i);
    for (unsigned int j = 0; j < width_ - 1; ++j) {
      size_t path_row = path.GetPath().at(j);
      int current = dataset.DatumAt(path_row, j);
      int east = dataset.DatumAt(path_row, j + 1);
      int echange = abs(current - east);
      int nechange = std::numeric_limits<int>::max();
      int sechange = std::numeric_limits<int>::max();
      if (path_row != 0) {
        int northeast = dataset.DatumAt(path_row - 1, j + 1);
        nechange = abs(current - northeast);
      }
      if (path_row != height_ - 1) {
        int southeast = dataset.DatumAt(path_row + 1, j + 1);
        sechange = abs(current - southeast);
      }
      if (nechange < echange && nechange < sechange) {
        path.SetLoc(j + 1, path_row - 1);
        path.IncEleChange(nechange);
      } else if ((sechange < echange && sechange < nechange) ||
                 (nechange == sechange && nechange < echange)) {
        path.SetLoc(j + 1, path_row + 1);
        path.IncEleChange(sechange);
      } else {
        path.SetLoc(j + 1, path_row);
        path.IncEleChange(echange);
      }
    }
    paths_.push_back(path);
    if (i == 0 || path.EleChange() < best_path_row.EleChange()) {
      best_path_row = path;
    }
  }
}

void PathImage::Coloring(std::vector<Path> paths_, Path const& best_path_row) {
  for (unsigned int i = 0; i < paths_.size(); ++i) {
    // iterating through path in each paths_ index
    std::vector<size_t> current_path = paths_.at(i).GetPath();
    for (size_t j = 0; j < paths_.at(i).Length(); ++j) {
      // index is the col
      size_t col = j;
      // number at(j) is the row
      size_t row = current_path.at(j);
      // change color in each image_path index to red RGB(252, 25, 63)
      path_image_.at(row).at(col) = kRed;
    }
  }

  // changing best_path_row to green
  for (size_t j = 0; j < best_path_row.Length(); ++j) {
    size_t col = j;
    size_t row = best_path_row.GetPath().at(j);
    path_image_.at(row).at(col) = kGreen;
  }
}

PathImage::PathImage(const GrayscaleImage& image,
                     const ElevationDataset& dataset) {
  width_ = image.Width();
  height_ = image.Height();

  // populates path_image_ with values from image
  path_image_ = image.GetImage();
  // std::cout << "copied image" << std::endl;

  // calcultes and stores all paths through image (ie dataset)
  Path best_path_row(width_, 0);
  PopulatePaths(width_, height_, dataset, best_path_row);
  // std::cout << "populated paths" << std::endl;

  // changing colors of image_path_ to have all red for the paths contained in
  // paths_ iterating through paths_ vector
  Coloring(paths_, best_path_row);
  // std::cout << "colored image" << std::endl;
}

size_t PathImage::Width() const { return width_; }

size_t PathImage::Height() const { return height_; }

unsigned int PathImage::MaxColorValue() const { return kMaxColorValue; }

const std::vector<Path>& PathImage::Paths() const {
  return paths_;  // does this return reference
}

const std::vector<std::vector<Color>>& PathImage::GetPathImage() const {
  return path_image_;  // does this return reference
}

void PathImage::ToPpm(const std::string& name) const {
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
      ofs << path_image_.at(i).at(j).Red() << " "
          << path_image_.at(i).at(j).Green() << " "
          << path_image_.at(i).at(j).Blue() << " ";
    }
    ofs << '\n';
  }
}
