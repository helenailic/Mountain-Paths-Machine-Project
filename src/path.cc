#include "path.hpp"

Path::Path(size_t length, size_t starting_row) {
  length_ = length;
  starting_row_ = starting_row;

  // initializes size_t vector (path) of length elements with 0s
  for (unsigned int i = 0; i < length_; ++i) {
    path_.push_back(0);
  }
}
size_t Path::Length() const { return length_; }

size_t Path::StartingRow() const { return starting_row_; }

unsigned int Path::EleChange() const { return ele_change_; }

void Path::IncEleChange(unsigned int value) { ele_change_ += value; }

const std::vector<size_t>& Path::GetPath() const {
  return path_;  // is this returning a reference
}
void Path::SetLoc(size_t col, size_t row) {
  path_.at(col) = row;  // is this correct?
}