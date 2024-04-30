// clang-format off
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//  Written By : Student Name                    Environment : ubuntu:bionic               //
//  Date ......: 2023/06/02                      Compiler ...: clang++                   //
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// clang-format on
/////////////////////////////////////////////////////////////////////////////////////////////
//                             Framework Set-up //
/////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CATCH_CONFIG_MAIN
#  undef CATCH_CONFIG_MAIN
#endif
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "catch.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
//                                 Includes //
/////////////////////////////////////////////////////////////////////////////////////////////
#include <stdexcept>

#include "color.hpp"
#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "path.hpp"
#include "path_image.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helpers/Constants Declarations //
/////////////////////////////////////////////////////////////////////////////////////////////
bool CompareFiles(const std::string& p1, const std::string& p2);

/////////////////////////////////////////////////////////////////////////////////////////////
//                                Color Examples //
/////////////////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Example: Color initialization", "[example_color_init]") {
  SECTION("Invalid red component") {
    REQUIRE_THROWS(Color(256, 128, 128));  // NOLINT
    REQUIRE_THROWS(Color(-1, 128, 128));   // NOLINT
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                                Elevation Data Examples //
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Example Elevation Data Test", "[ed-forward-2-3]") {
  const std::string kTestFile = "./example-data/ex_input_data/forward-2-3.dat";
  constexpr size_t kTestFileWidth = 2;
  constexpr size_t kTestFileHeight = 3;
  const std::vector<std::vector<int>> kElevationDataGroundTruth = {
      {9, 12}, {8, 10}, {7, 14}};
  ElevationDataset ed(kTestFile, kTestFileWidth, kTestFileHeight);
  const auto& student_elevation_data = ed.GetData();

  // Are the vectors kElevationDataGroundTruth and student_elevation_data
  // identical? If so, pass test.
  REQUIRE(std::equal(student_elevation_data.begin(),
                     student_elevation_data.end(),
                     kElevationDataGroundTruth.begin()));
}

TEST_CASE("ed_too_little", "[ed_too_little]") {
  const std::string kTestFile = "example-data/all-tie-row1-2w-3h.dat";
  constexpr size_t kTestFileWidth = 100;
  constexpr size_t kTestFileHeight = 100;
  REQUIRE_THROWS(ElevationDataset(kTestFile, kTestFileWidth, kTestFileHeight));
}

TEST_CASE("ed_too_much", "[ed_too_much]") {
  const std::string kTestFile = "example-data/all-tie-row1-2w-3h.dat";
  constexpr size_t kTestFileWidth = 1;
  constexpr size_t kTestFileHeight = 1;
  REQUIRE_THROWS(ElevationDataset(kTestFile, kTestFileWidth, kTestFileHeight));
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                              Grayscale Image Examples //
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Example Grayscale Data Test", "[gi-forward-2-3]") {
  const std::string kTestFile = "./example-data/ex_input_data/forward-2-3.dat";
  constexpr size_t kTestFileWidth = 2;
  constexpr size_t kTestFileHeight = 3;
  const std::vector<std::vector<Color>> kImageGroundTruth = {
      {Color(73, 73, 73), Color(182, 182, 182)},
      {Color(36, 36, 36), Color(109, 109, 109)},
      {Color(0, 0, 0), Color(255, 255, 255)}};

  ElevationDataset ed(kTestFile, kTestFileWidth, kTestFileHeight);
  GrayscaleImage gi(ed);
  const auto& student_image_data = gi.GetImage();

  // Are the vectors kImageGroundTruth and student_image_data
  // identical? If so, pass test.
  REQUIRE(std::equal(student_image_data.begin(),
                     student_image_data.end(),
                     kImageGroundTruth.begin()));
}

TEST_CASE("PPM Example Test: Grayscale image from prompt_5w_2h.ppm",
          "[ppm_ex_grayscale_prompt_5w_2h]") {
  const std::string kStudentOutput =
      "./student_out/ex_grayscale_prompt_5w_2h.ppm";
  const std::string kTestFile = "./example-data/ex_input_data/prompt_5w_2h.dat";
  const std::string kGroundTruth =
      "./example-data/ex_output_grayscale/prompt_5w_2h.ppm";
  constexpr size_t kTestFileWidth = 5;
  constexpr size_t kTestFileHeight = 2;
  ElevationDataset ed(kTestFile, kTestFileWidth, kTestFileHeight);
  GrayscaleImage gi(ed);
  gi.ToPpm(kStudentOutput);
  REQUIRE(CompareFiles(kStudentOutput, kGroundTruth));
}

TEST_CASE("Example Test: Grayscale image from map-input-w51-h55.dat",
          "ex_gray_image_map-input-w51-h55.dat") {
  const std::string kStudentOutput =
      "./student_out/ex_path_image_map-input-w51-h55.ppm";
  const std::string kTestFile =
      "./example-data/ex_input_data/map-input-w51-h55.dat";
  const std::string kGroundTruth =
      "./example-data/ex_output_grayscale/map-input-w51-h55.ppm";
  constexpr size_t kTestFileWidth = 51;
  constexpr size_t kTestFileHeight = 55;
  ElevationDataset ed(kTestFile, kTestFileWidth, kTestFileHeight);
  GrayscaleImage gi(ed);
  gi.ToPpm(kStudentOutput);
  REQUIRE(CompareFiles(kStudentOutput, kGroundTruth));
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                              Computing Paths: Navigation Examples //
/////////////////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Example Navigation Test: Go forward.", "[forward-2-3]") {
  const std::string kTestFile = "./example-data/ex_input_data/forward-2-3.dat";
  constexpr size_t kTestFileWidth = 2;
  constexpr size_t kTestFileHeight = 3;
  const std::vector<size_t> kPathToEvaluateGroundTruth = {1, 1};
  ElevationDataset ed(kTestFile, kTestFileWidth, kTestFileHeight);
  GrayscaleImage gi(ed);
  PathImage pi(gi, ed);

  // Let's get the path's vector from `pi`
  const auto& student_paths = pi.Paths();

  // We want to analyze the program's behavior on the path starting from Row 1,
  // so let's get the path we created for that row.
  const auto& student_path_to_evaluate = student_paths.at(1).GetPath();

  // Now let's compare that path to the expected path stored in
  // kPathToEvaluateGroundTruth.
  REQUIRE(std::equal(student_path_to_evaluate.begin(),
                     student_path_to_evaluate.end(),
                     kPathToEvaluateGroundTruth.begin()));
}

TEST_CASE("Example Navigation Test: Go Up.", "[forward-up-2-3]") {
  const std::string kTestFile =
      "./example-data/ex_input_data/forward-up-2-3.dat";
  constexpr size_t kTestFileWidth = 2;
  constexpr size_t kTestFileHeight = 3;
  const std::vector<size_t> kPathToEvaluateGroundTruth = {1, 0};
  ElevationDataset ed(kTestFile, kTestFileWidth, kTestFileHeight);
  GrayscaleImage gi(ed);
  PathImage pi(gi, ed);
  const auto& student_paths = pi.Paths();
  const auto& student_path_to_evaluate = student_paths.at(1).GetPath();
  REQUIRE(std::equal(student_path_to_evaluate.begin(),
                     student_path_to_evaluate.end(),
                     kPathToEvaluateGroundTruth.begin()));
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                              Path Image Examples //
/////////////////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Example Test: Path image from prompt_5w_2h.ppm",
          "ex_path_image_prompt_5w_2h]") {
  const std::string kStudentOutput =
      "./student_out/ex_path_image_prompt_5w_2h.ppm";
  const std::string kTestFile = "./example-data/ex_input_data/prompt_5w_2h.dat";
  const std::string kGroundTruth =
      "./example-data/ex_output_paths/prompt_5w_2h.ppm";
  constexpr size_t kTestFileWidth = 5;
  constexpr size_t kTestFileHeight = 2;
  ElevationDataset ed(kTestFile, kTestFileWidth, kTestFileHeight);
  GrayscaleImage gi(ed);
  PathImage pi(gi, ed);
  pi.ToPpm(kStudentOutput);
  REQUIRE(CompareFiles(kStudentOutput, kGroundTruth));
}

TEST_CASE("Example Test: Path image from map-input-w51-h55.dat",
          "ex_path_image_map-input-w51-h55.dat") {
  const std::string kStudentOutput =
      "./student_out/ex_path_image_map-input-w51-h55.ppm";
  const std::string kTestFile =
      "./example-data/ex_input_data/map-input-w51-h55.dat";
  const std::string kGroundTruth =
      "./example-data/ex_output_paths/map-input-w51-h55.ppm";
  constexpr size_t kTestFileWidth = 51;
  constexpr size_t kTestFileHeight = 55;
  ElevationDataset ed(kTestFile, kTestFileWidth, kTestFileHeight);
  GrayscaleImage gi(ed);
  PathImage pi(gi, ed);
  pi.ToPpm(kStudentOutput);
  REQUIRE(CompareFiles(kStudentOutput, kGroundTruth));
}

/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helper Definitions //
/////////////////////////////////////////////////////////////////////////////////////////////

bool CompareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1);
  std::ifstream f2(p2);

  if (f1.fail() || f2.fail()) {
    return false;  // file problem
  }

  std::string f1_read;
  std::string f2_read;
  while (f1.good() || f2.good()) {
    f1 >> f1_read;
    f2 >> f2_read;
    if (f1_read != f2_read || (f1.good() && !f2.good()) ||
        (!f1.good() && f2.good()))
      return false;
  }
  return true;
}
