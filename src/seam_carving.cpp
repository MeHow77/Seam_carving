#include <numeric>
#include "seam_carving.h"
cv::Mat calc_e1(const cv::Mat &image){
    int ddepth = CV_16S;
    int ksize = 5;

    cv::Mat srcBlur, srcGray, e1;
    cv::GaussianBlur(image, srcBlur, cv::Size(3,3), 0, 0);

    cv::Mat gradX, gradY;
    cv::Mat absGradX, absGradY;

    cv::Scharr(image, gradX, ddepth, 1, 0);
    cv::Scharr(image, gradY, ddepth, 0, 1);

    cv::convertScaleAbs(gradX, absGradX);
    cv::convertScaleAbs(gradY, absGradY);

    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, e1);
    return e1;
}

cv::Mat calcGrad(const cv::Mat &image){
    cv::Mat srcBlur;
    cv::GaussianBlur(image, srcBlur, cv::Size(3,3), 0, 0);
    if (srcBlur.channels()==3){
        cv::cvtColor(srcBlur, srcBlur, cv::COLOR_BGR2GRAY);
    }
    int kernelY[9] = {3,0,-3,10,0,-10,3,0,3};
    float kernelX[9] = {3,10,3,0,0,0,-3,-10,-3};
    cv::Mat filterY(3, 3, CV_32F, kernelY);
    cv::Mat filterX(3, 3, CV_32F, kernelX);
    cv::Mat dstx, dsty, absY, absX, gradMat;
    filter2D(srcBlur,dsty, CV_64F,filterY, cv::Point( -1, -1 ),0, cv::BORDER_DEFAULT );
    filter2D(srcBlur,dstx, CV_64F,filterX, cv::Point( -1, -1 ),0, cv::BORDER_DEFAULT );
    cv::convertScaleAbs(dstx, absX);
    cv::convertScaleAbs(dsty, absY);
    cv::add(absY, absX, gradMat);
    return gradMat;
}

cv::Mat verticalCumulativeMat(const cv::Mat &eMat){
    int nRows = eMat.rows;
    int nCols = eMat.cols;
    int a, b, c;

    cv::Mat res (eMat.rows, eMat.cols, CV_32S, cv::Scalar(0));
    eMat.row(0).copyTo(res.row(0));

    std::vector<int> validVals;
    for(int row=1; row<nRows; row++){
        for (int col=0; col<nCols; col++) {
            a = res.at<int>(row-1, std::max(col-1, 0));
            b = res.at<int>(row-1, col);
            c = res.at<int>(row-1, std::min(col+1,  nCols-1));

            res.at<int>(row, col) = eMat.at<int>(row, col) + std::min(a, std::min(b, c));
        }
    }
    return res;
}

cv::Mat horizontalCumulativeMat(const cv::Mat &image){
    cv::Mat rotated;
    cv::rotate(image, rotated, cv::ROTATE_90_CLOCKWISE);
    rotated = verticalCumulativeMat(rotated);
    cv::rotate(rotated, rotated, cv::ROTATE_90_COUNTERCLOCKWISE);
    return rotated;
}

std::vector<std::pair<int, int>> findVerticalSeam(const cv::Mat &eMat){
    auto lastRow = eMat.row(eMat.rows - 1);
    std::vector<std::pair<int,int>> seam;
    int minIndex = getMinimumIndex<int>(lastRow);
    seam.emplace_back(eMat.rows-1, minIndex); //seam's beginning

    for (auto i=eMat.rows-2; i>=0; i--){
        try {
            seam.push_back(validPair(eMat, seam.back()));
        }catch (const std::runtime_error& e) {
            std::cout << e.what() << '\n';
        }
    }
    std::reverse(seam.begin(), seam.end());
    return seam;
}

std::pair<int,int> validPair(const cv::Mat &eMat, std::pair<int,int> prevPair) {
     int pRow = prevPair.first;
     int pCol = prevPair.second;
     int cRow = pRow-1;
     int tCols = eMat.cols;

     if (cRow < 0){
         throw std::runtime_error("Invalid seam pair");
     }
     int minCol = pCol;
     int minVal = eMat.at<int>(cRow, pCol);
     if (pCol > 0 && eMat.at<int>(cRow, pCol-1) < minVal){
         minCol = pCol-1;
     }
     if (pCol < tCols-1 && eMat.at<int>(cRow, pCol+1) < minVal){
        minCol = pCol+1;
     }
     return std::make_pair(cRow, minCol);
}

std::vector<std::pair<int, int>> findHorizontalSeam(const cv::Mat &eMat) {
    cv::Mat rotated;
    std::vector<std::pair<int,int>> seam;
    cv::rotate(eMat, rotated, cv::ROTATE_90_CLOCKWISE);
    seam = findVerticalSeam(rotated);
    for(auto &p : seam){
        int pRow = p.first;
        int pCol = p.second;
        p.second = pRow;
        p.first = rotated.cols-1 - pCol;
    }
    return seam;
}

//std::vector<std::pair<int, int>> findVerticalSeam(const cv::Mat &eMat, int minIdx){
//    auto lastRow = eMat.row(eMat.rows - 1);
//    std::vector<std::pair<int,int>> seam;
//    //int minIndex = getMinimumIndex(lastRow);
//    seam.emplace_back(eMat.rows-1, minIdx); //seam's beginning
//
//    for (auto i=eMat.rows-2; i>=0; i--){
//        try {
//            seam.push_back(validPair(eMat, seam.back()));
//        }catch (const std::runtime_error& e) {
//            std::cout << e.what() << '\n';
//        }
//    }
//    std::reverse(seam.begin(), seam.end());
//    return seam;
//}

//std::vector<int> partialSortIndexes(const std::vector<int> &v, int sortRange) {
//    std::vector<int> idx(v.size());
//    iota(idx.begin(), idx.end(), 0);
//    std::partial_sort(idx.begin(), idx.begin()+sortRange, idx.end(),
//                      [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});
//    return idx;
//}

cv::Mat carveVerticalSeam(const cv::Mat &eMat, const std::vector<std::pair<int, int>> &seam){
    int rows = eMat.rows;
    int cols = eMat.cols;
    cv::Mat carved (eMat.rows, eMat.cols-1, eMat.type());
    for (int i = 0; i < rows; ++i){
        auto* dest_row = carved.ptr<int>(i);
        const auto* source_row = eMat.ptr<int>(i);
        for (auto j=0; j < seam[i].second; j++){
            dest_row[j] = source_row[j];
        }
        for (auto j=seam[i].second; j < cols-1; j++){
            dest_row[j] = source_row[j+1];
        }
    }
    return carved;
}

cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size) {
  (void)out_size;
  return image;
}
