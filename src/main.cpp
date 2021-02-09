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

    e1 = calc_e1(image);
    auto hog = calcHOG(image, e1);
    m = verticalCumulativeMat(hog);

    int carveScale = 500;
    auto start = std::chrono::high_resolution_clock::now();

    auto seamsStarts = partialSortIndexes(m.row(m.rows-1), carveScale);
    std::vector<std::vector<std::pair<int,int>>> seams;
    for (const auto seamStart:seamsStarts){
        seams.push_back(findVerticalSeam(m, seamStart));
//        image = carveVerticalSeam<uchar>(image, seams[i]);
    }
    for (int i=0; i<seamsStarts.size(); i++){
        for (auto const &pixel : seams[i]) {
            cv::circle(image, cv::Point(pixel.second, pixel.first), 1, CV_RGB(255, 0, 0), 1);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Time taken by function: "
         << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count() << " microseconds" << std::endl;
    display_img(image);

  return 0;
}


