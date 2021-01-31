#include <opencv2/highgui.hpp>
#include <iostream>

#include "seam_carving.h"

int main(int, char** argv) {
  const auto in = cv::imread(argv[1]);
  const auto out = seamCarving(in, cv::Size(641, 480));
  cv::imwrite("/home/dev/seam-carving/src/output.png", out);
  return 0;
}
