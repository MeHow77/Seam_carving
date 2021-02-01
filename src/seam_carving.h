#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

cv::Mat calc_e1(const cv::Mat &image);
cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size);
