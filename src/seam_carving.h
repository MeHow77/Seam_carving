#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

cv::Mat calc_e1(const cv::Mat &image);
cv::Mat verticalCumulativeMat(const cv::Mat &image);
uchar getMinimum(const std::vector<uchar>& vals);
cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size);
