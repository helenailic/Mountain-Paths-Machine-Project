#include <iostream>

#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "path_image.hpp"

int main() {
  const std::string kTestInputFile =
      "./example-data/ex_input_data/prompt_5w_2h.dat";
  constexpr size_t kTestInputFileWidth = 5;
  constexpr size_t kTestInputFileHeight = 2;

  ElevationDataset ed(
      kTestInputFile, kTestInputFileWidth, kTestInputFileHeight);
  GrayscaleImage gi2(ed);
  PathImage pi(gi2, ed);
  pi.ToPpm("./student_out/ex_path_image_prompt_5w_2h.ppm");
}

// read in elevation data, process it, print out grayscale topological image map
// will have multiple potential paths drawn in red with best rec path in green

// read in elevation data in white-space delimited list of int values
// each int is avg elevation in meters of cell in grid
// list of ints maps directly onto rectangular grid
