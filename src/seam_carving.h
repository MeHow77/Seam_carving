#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

cv::Mat calc_e1(const cv::Mat &image);
cv::Mat verticalCumulativeMat(const cv::Mat &image);
cv::Mat horizontalCumulativeMat(const cv::Mat &image);
uchar getMinimum(const std::vector<uchar>& vals);
int getMinimumIndex(const std::vector<uchar>& vals);
std::pair<int, int> validPair(const cv::Mat &eMat, std::pair<int,int> prevPair);
cv::Mat carveVerticalSeam(const cv::Mat &eMat, const std::vector<std::pair<int, int>> &seam);
std::vector<int> partialSortIndexes(const std::vector<int> &v, int sortRange);
std::vector<std::pair<int, int>> findVerticalSeam(const cv::Mat &eMat);
std::vector<std::pair<int, int>> findHorizontalSeam(const cv::Mat &eMat);

cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size);
