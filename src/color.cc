#include "color.hpp"

#include <stdexcept>

Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  // need to ensure valid color has been constructed...
  if (red_ < kColorValueMin || red_ > kColorValueMax ||
      green_ < kColorValueMin || green_ > kColorValueMax ||
      blue_ < kColorValueMin || blue_ > kColorValueMax) {
    throw std::runtime_error("this is not a valid color");
  }
}

// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}