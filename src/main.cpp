#include <opencv2/highgui.hpp>
#include <iostream>

#include "seam_carving.h"

void display_img(const cv::Mat &image){
    cv::namedWindow("Basic_display", cv::WINDOW_AUTOSIZE);
    cv::imshow("Basic_display", image);
    cv::waitKey(0);
}
int main(int, char** argv) {
    const auto in = cv::imread(argv[1]);
    auto image = in.clone();
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::Mat e1, m;

    int carveScale = 100;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0; i<carveScale; i++){
        e1 = calc_e1(image);
        m = verticalCumulativeMat(e1);
        auto seam = findVerticalSeam(m);
        image = carveVerticalSeam<uchar>(image, seam);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Time taken by function: "
         << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count() << " microseconds" << std::endl;
    display_img(image);

  return 0;
}


