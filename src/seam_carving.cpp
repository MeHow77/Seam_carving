#include "seam_carving.h"
cv::Mat calc_e1(const cv::Mat &image){
    int ddepth = CV_16S;
    int ksize = 5;

    cv::Mat srcBlur, srcGray, e1;
    cv::GaussianBlur(image, srcBlur, cv::Size(3,3), 0, 0);
    cv::cvtColor(srcBlur, srcGray, cv::COLOR_BGR2GRAY);

    cv::Mat gradX, gradY;
    cv::Mat absGradX, absGradY;

    cv::Sobel(srcGray, gradX, ddepth, 1, 0, ksize);
    cv::Sobel(srcGray, gradY, ddepth, 0, 1, ksize);

    cv::convertScaleAbs(gradX, absGradX);
    cv::convertScaleAbs(gradY, absGradY);

    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, e1);
    return e1;
}

cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size) {
  (void)out_size;
  return image;
}
