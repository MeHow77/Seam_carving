#include <opencv2/highgui.hpp>
#include <iostream>
#include <chrono>

#include "seam_carving.h"

void display_img(const cv::Mat &image){
    cv::namedWindow("Basic_display", cv::WINDOW_AUTOSIZE);
    cv::imshow("Basic_display", image);
    cv::waitKey(0);
}
int main(int, char** argv) {
    const auto in = cv::imread(argv[1]);
    auto image = in.clone();
    cv::Mat imageGray;
    cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
    cv::Mat e1, m;

    int carveScale = 200;
    auto begin = std::chrono::high_resolution_clock::now();
    for (int i=0; i<carveScale; i++){
        e1 = sc::calc_e1(imageGray);
        m = sc::verticalCumulativeMat(e1);
        auto seam = sc::findVerticalSeam(m);
        image = sc::carveVerticalSeam<uchar>(image, seam);
        imageGray = sc::carveVerticalSeam<uchar>(imageGray, seam);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
    display_img(image);

  return 0;
}


