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

cv::Mat verticalCumulativeMat(const cv::Mat &image){
    // DP: cv::Mat cache(cv::Size(image.rows,image.cols), CV_8UC1, cv::Scalar(0));
    //cache.at<uchar>(0,0) = 1;
    int nRows = image.rows;
    int nCols = image.cols;

    cv::Mat res = image.clone();
    const uchar* row;
    std::vector<uchar> validVals;

    for(int i=1; i<nRows; i++){
        row = res.ptr<uchar>(i - 1);
        validVals = {row[0], row[1]};
        res.at<uchar>(i,0) += getMinimum(validVals);
        for(int j=1; j<nCols-1; j++) {
            validVals = {row[j - 1], row[j], row[j+1]};
            res.at<uchar>(i,j) += getMinimum(validVals);
        }
        validVals = {row[nCols-2], row[nCols-1]};
        res.at<uchar>(i,nCols-1) += getMinimum(validVals);
    }
    return res;
}

uchar getMinimum(const std::vector<uchar>& vals){
    auto min = *std::min_element(vals.begin(), vals.end());
    return min;
}



cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size) {
  (void)out_size;
  return image;
}
