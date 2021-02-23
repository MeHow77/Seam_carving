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
    //SIMPLE INTERFACE
    const auto in = cv::imread(argv[1]);
    if (in.empty()){
        std::cout<<"Image reading failed, returning";
        return 0;
    }
    int isVertical = 0;
    std::cout<<"Insert 1 for width reduction or 0 for high reduction"<<std::endl;
    std::cin >> isVertical;

    int carveSize = 0;
    std::cout<<"Insert number of pixels to carve"<<std::endl;
    std::cin>>carveSize;

    if (isVertical){
        if (carveSize >= in.rows){
            std::cout<<"Number of rows to carve exceeds current size, returning"<<std::endl;
            return 0;
        }
    }else{
        if (carveSize >= in.cols){
            std::cout<<"Number of columns to carve exceeds current size, returning"<<std::endl;
            return 0;
        }
    }

    auto begin = std::chrono::high_resolution_clock::now();
    auto image = sc::seamCarving(in, carveSize, isVertical);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Image carved in: %.3f seconds.\n", elapsed.count() * 1e-9);

    display_img(image);

  return 0;
}


