#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "../seam_carving.cpp"

bool compareMatrices(const cv::Mat &mat1, const cv::Mat &mat2){
    cv::Mat diff;
    cv::compare(mat1, mat2, diff, cv::CMP_NE);
    return cv::countNonZero(diff) == 0;
}


TEST(testMat, verticalSum){
    uchar data[3][5] = {{1, 1, 5, 7, 4}, {5, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};
    int summed[3][5] ={{1, 1, 5, 7, 4}, {6, 8, 9, 13, 14}, {17, 18, 21, 23, 28}};
    cv::Mat A = cv::Mat(3,5, CV_8UC1, data);
    cv::Mat summedA = cv::Mat(3,5, CV_32S, summed);
    cv::Mat output;
    sc::verticalCumulativeMat(A, output);
    ASSERT_TRUE(compareMatrices(output,summedA));
}

TEST(testMat, horizontalSum){
    uchar data[3][5] = {{1, 1, 5, 7, 4}, {5, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};
    int summed[3][5] = {{1, 2, 7, 14, 18}, {5, 8, 10, 16, 24}, {11, 17, 21, 24, 31}};

    cv::Mat A = cv::Mat(3,5, CV_8UC1, data);
    cv::Mat summedA = cv::Mat(3,5, CV_32S, summed);
    cv::Mat output;
    sc::horizontalCumulativeMat(A, output);
    ASSERT_TRUE(compareMatrices(output,summedA));
}

TEST(testMat, verticalSeam){
    int dataI[5][4] = {{3, 6, 2, 5}, {12, 11, 10, 8}, {15, 16, 14, 18}, {25, 19, 30, 25}, {23, 30, 26, 29}};
    cv::Mat I = cv::Mat(5,4, CV_32S, dataI);
    std::vector<std::pair<int, int>> seamResult {{0,2}, {1,3}, {2,2}, {3,1}, {4,0}};

    auto seamOutput = sc::findVerticalSeam(I);
    bool res = seamOutput == seamResult;
    ASSERT_TRUE(res);
}

TEST(testMat, horizontalSeam){
    int dataI[3][5] = {{1, 2, 7, 14, 18}, {5, 8, 10, 16, 24}, {11, 17, 21, 24, 31}};
    cv::Mat I = cv::Mat(3,5, CV_32S, dataI);
    std::vector<std::pair<int, int>> seamResult {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}};

    auto seamOutput = sc::findHorizontalSeam(I);
    bool res = seamOutput == seamResult;
    ASSERT_TRUE(res);
}

TEST(testMat, carveSeam){
    int dataI[5][4] = {{3, 6, 2, 5}, {12, 11, 10, 8}, {15, 16, 14, 18}, {25, 19, 30, 25}, {23, 30, 26, 29}};
    cv::Mat I = cv::Mat(5,4, CV_32S, dataI);
    std::vector<std::pair<int, int>> seamToCut {{0,2}, {1,3}, {2,2}, {3,1}, {4,0}};

    int dataCarved[5][3] = {{3, 6, 5}, {12, 11, 10}, {15, 16, 18}, {25, 30, 25}, { 30, 26, 29}};
    cv::Mat carvedCorrect = cv::Mat(5,3, CV_32S, dataCarved);

    auto carvedOutput = sc::carveVerticalSeam(I, seamToCut);
    ASSERT_TRUE(compareMatrices(carvedOutput, carvedCorrect));
}
