#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

cv::Mat calc_e1(const cv::Mat &image);
cv::Mat verticalCumulativeMat(const cv::Mat &image);
cv::Mat horizontalCumulativeMat(const cv::Mat &image);
uchar getMinimum(const std::vector<uchar>& vals);
uchar getMinimumIndex(const std::vector<uchar>& vals);
cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size);
cv::Mat findMinimalVerticalSeam(const cv::Mat &eMat);
cv::Mat findMinimalHorizontalSeam(const cv::Mat &eMat);
