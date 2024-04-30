#include "elevation_dataset.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// translate file rep of data into ElevationDataset Object
// constructor takes path to pain-text file to be read
// and composition of dataset (w and h)
// initialize width_ height_ with params
// write elevation data to data_ in [row, col] form
// initialize data_ vectr as preallocated height rows and width cols
// then use nested for statement to read in values (look at example)
// record min and max elevation values in dataset in min_ele_ and max_ele_

// while translating file to matrix form...
// validate user input
// # elevation pts in file should width * height --> else throw exception
// if error from formatted-read failure --> throw exception (std::runtime_error)
// with descrip

ElevationDataset::ElevationDataset(const std::string& filename,
                                   size_t width,
                                   size_t height) {
  width_ = width;
  height_ = height;

  std::ifstream ifs(filename);
  int num = 0;
  for (unsigned int i = 0; i < height_; ++i) {
    std::vector<int> temp_row;
    for (unsigned int j = 0; j < width_; ++j) {
      ifs >> num;
      if (ifs.fail()) {
        throw std::runtime_error("format read error");
      }
      temp_row.push_back(num);

      if (i == 0 && j == 0) {
        max_ele_ = num;
        min_ele_ = num;
      } else if (num > max_ele_) {
        max_ele_ = num;
      } else if (num < min_ele_) {
        min_ele_ = num;
      }
    }
    data_.push_back(temp_row);
  }
  if (ifs >> num) {
    throw std::runtime_error("too much data in file");
  }
}

size_t ElevationDataset::Width() const { return width_; }

size_t ElevationDataset::Height() const { return height_; }

int ElevationDataset::MaxEle() const { return max_ele_; }

int ElevationDataset::MinEle() const { return min_ele_; }

int ElevationDataset::DatumAt(size_t row, size_t col) const {
  return data_.at(row).at(col);  // is this returning by reference?
}

const std::vector<std::vector<int>>& ElevationDataset::GetData() const {
  return data_;  // is this returning a reference? yes
}
