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
    auto e1 = calcGrad(in);
    auto m = verticalCumulativeMat(e1);

    int carveScale = 200;
    //auto seamsStart = partialSortIndexes(m.row(m.rows-1), carveScale);
    for (int i=0; i<carveScale; i++){
        auto seam = findVerticalSeam(m);
        m = carveVerticalSeam(m, seam);
        e1 = carveVerticalSeam(e1, seam);
    }
//    for (auto p : seam){
//        cv::circle(in, cv::Point(p.second, p.first), 1, CV_RGB(255, 0, 0), 1);
//    }
    display_img(in);
    display_img(e1);

  return 0;
}


