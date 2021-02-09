#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
namespace sc {
    //energyFuncs
    cv::Mat calc_e1(const cv::Mat &inputImg);

    //cumulative sums
    cv::Mat verticalCumulativeMat(const cv::Mat &inputEnergyMat);
    cv::Mat horizontalCumulativeMat(const cv::Mat &inputEnergyMat);

    template<typename T>
    cv::Mat carveVerticalSeam(const cv::Mat &eMat,
                              const std::vector<std::pair<int, int>> &seam) {
        /** @brief carves a seam from an energy matrix/image
         * handles multichannel input, each channel separately
         *
        @param eMat: CV_32SC1 (multichannel) input array of cumulative energy/image
        @param seam: vector of std::pairs<row,col>; seam's path
        @return outputMat: cloned eMat with no given seam
        */
        int rows = eMat.rows;
        int cols = eMat.cols;
        bool isMultichannel = eMat.channels() > 1;
        cv::Mat outputMat = cv::Mat(eMat.rows, eMat.cols-1, eMat.type(), cv::Scalar(0));

        std::vector<cv::Mat> inCh;
        std::vector<cv::Mat> outCh;
        //split multichannel array to single arrays
        if (isMultichannel){
            cv::split(eMat, inCh);
            cv::split(outputMat, outCh);
        }
        else{
            inCh.push_back(eMat);
            outCh.push_back(outputMat);
        }
        cv::Mat *currOutCh;
        cv::Mat *currInCh;
        //carve seam from each channel separately
        for(auto c=0; c<inCh.size(); c++) {
            currOutCh = &outCh[c];
            currInCh = &inCh[c];
            for (int i = 0; i < rows; ++i) {
                auto *dest_row = currOutCh->ptr<T>(i);
                const auto *source_row = currInCh->ptr<T>(i);
                for (auto j = 0; j < seam[i].second; j++) {
                    dest_row[j] = source_row[j];
                }
                for (auto j = seam[i].second; j < cols - 1; j++) {
                    dest_row[j] = source_row[j + 1];
                }
            }
        }
        if (isMultichannel){
            cv::merge(outCh, outputMat);
        }
        return outputMat;
    }

    template <typename T>
    cv::Mat carveHorizontalSeam(const cv::Mat &eMat,
                              const std::vector<std::pair<int, int>> &seam) {
        cv::Mat rotated;
        cv::rotate(eMat, rotated, cv::ROTATE_90_CLOCKWISE);
        rotated = carveVerticalSeam<T>(rotated, seam);
        cv::rotate(rotated, rotated, cv::ROTATE_90_COUNTERCLOCKWISE);
        return rotated;
    }

    std::vector<std::pair<int, int>> findVerticalSeam(const cv::Mat &eMat);
    std::vector<std::pair<int, int>> findHorizontalSeam(const cv::Mat &eMat);

//utility
    template<typename T>
    int getMinimumIndex(const std::vector<T> &vals) {
        auto min = std::min_element(vals.begin(), vals.end()) - vals.begin();
        return min;
    }
    std::vector<std::pair<int, int>> rotSeamCntClk(const std::vector<std::pair<int, int>> &seam, const int &colsNo);

    std::pair<int, int> validPair(const cv::Mat &eMat, std::pair<int, int> prevPair);

    cv::Mat seamCarving(const cv::Mat &image, const cv::Size &out_size);
}
