#ifndef COLOR_H
#define COLOR_H

class Color {
public:
  Color() = default;  // does this already set it to all zero values?
  Color(int r, int g, int b);
  int Red() const { return red_; }
  int Green() const { return green_; }
  int Blue() const { return blue_; };

private:
  static constexpr int kColorValueMin = 0;
  static constexpr int kColorValueMax = 255;
  int red_ = 0;
  int green_ = 0;
  int blue_ = 0;
};

bool operator==(const Color& rhs, const Color& lhs);

#endif