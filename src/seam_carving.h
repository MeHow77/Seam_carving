#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
namespace sc {
    //energyFuncs
    void calc_e1(const cv::Mat &inputImg, cv::Mat &outputMat);

    //cumulative sums
    void verticalCumulativeMat(const cv::Mat &inputEnergyMat, cv::Mat &outputCumulativeMat);
    void horizontalCumulativeMat(const cv::Mat &inputEnergyMat, cv::Mat &outputCumulativeMat);

    template<typename T>
    cv::Mat carveVerticalSeam(const cv::Mat &eMat, const std::vector<std::pair<int, int>> &seam) {
        int rows = eMat.rows;
        int cols = eMat.cols;
        cv::Mat carved(eMat.rows, eMat.cols - 1, eMat.type());
        for (int i = 0; i < rows; ++i) {
            auto *dest_row = carved.ptr<T>(i);
            const auto *source_row = eMat.ptr<T>(i);
            for (auto j = 0; j < seam[i].second; j++) {
                dest_row[j] = source_row[j];
            }
            for (auto j = seam[i].second; j < cols - 1; j++) {
                dest_row[j] = source_row[j + 1];
            }
        }
        return carved;
    }

    std::vector<std::pair<int, int>> findVerticalSeam(const cv::Mat &eMat);

    std::vector<std::pair<int, int>> findHorizontalSeam(const cv::Mat &eMat);

//utility
    std::vector<int> partialSortIndexes(const std::vector<int> &v, int sortRange);

    template<typename T>
    T getMinimum(const std::vector<T> &vals) {
        auto min = *std::min_element(vals.begin(), vals.end());
        return min;
    }

    template<typename T>
    int getMinimumIndex(const std::vector<T> &vals) {
        auto min = std::min_element(vals.begin(), vals.end()) - vals.begin();
        return min;
    }

    std::pair<int, int> validPair(const cv::Mat &eMat, std::pair<int, int> prevPair);

    cv::Mat seamCarving(const cv::Mat &image, const cv::Size &out_size);
}