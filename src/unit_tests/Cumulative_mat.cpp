#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "../seam_carving.cpp"


TEST(testMat, verticalSum){
    int data[3][5] = {{1, 1, 5, 7, 4}, {5, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};
    int summed[3][5] ={{1, 1, 5, 7, 4}, {6, 8, 10, 13, 14}, {17, 18, 21, 24, 28}};
    cv::Mat A = cv::Mat(3,5, CV_8UC1, data);
    cv::Mat summedA = cv::Mat(3,5, CV_8UC1, summed);

    cv::Mat funcA = cumulativeMat(A);
    cv::Mat diff;
    cv::compare(summedA, funcA, diff, cv::CMP_NE);
    bool eq = cv::countNonZero(diff) == 0;
    ASSERT_TRUE(eq);
}