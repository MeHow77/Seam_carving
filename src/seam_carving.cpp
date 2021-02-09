#include "seam_carving.h"
namespace sc {
    cv::Mat calc_e1(const cv::Mat &inputImg) {
    /** @brief Blurs, calculates sum of absolute values of gradients, adds them and converts the result to 32-bit C1.
    @param inputImg: BGR input array
    @return outputMat: output array containing sum of absolute gradients
    */
        int ddepth = CV_16S;
        cv::Mat outputMat;
        if (inputImg.channels() == 3){
            cv::cvtColor(inputImg, outputMat, cv::COLOR_BGR2GRAY);
        }
        else {
            outputMat = inputImg.clone();
        }

        cv::Mat inputBlur, inputGray;
        cv::GaussianBlur(outputMat, inputBlur, cv::Size(3, 3), 0, 0);

        cv::Mat gradX, gradY;
        cv::Mat absGradX, absGradY;
        //X,Y gradients
        cv::Scharr(inputBlur, gradX, ddepth, 1, 0);
        cv::Scharr(inputBlur, gradY, ddepth, 0, 1);
        // |X'| + |Y'|
        cv::convertScaleAbs(gradX, absGradX);
        cv::convertScaleAbs(gradY, absGradY);
        cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, outputMat);
        return outputMat;
    }

    cv::Mat verticalCumulativeMat(const cv::Mat &inputEnergyMat) {
    /** @brief sums from top to bottom in the DP manner: M(i,j) = e(i,j) + min(M(i-1,j-1), M(i-1,j), M(i-1, j+1))
    @param inputEnergyMat: CV_8UC1 input array
    @return dst: CV_32SC1 output array containing cumulative sum for seams searching
    */
        int nRows = inputEnergyMat.rows;
        int nCols = inputEnergyMat.cols;
        int left, middle, right;

        cv::Mat dst;
        dst = inputEnergyMat.clone();
        dst.convertTo(dst, CV_32S);

        std::vector<int> validVals;
        const int *prevRow;
        int *currRow;
        for (int row = 1; row < nRows; row++) {
            prevRow = dst.ptr<int>(row - 1);
            currRow = dst.ptr<int>(row);
            for (int col = 0; col < nCols; col++) {
                left = prevRow[std::max(col - 1, 0)];
                middle = prevRow[col];
                right = prevRow[std::min(col + 1, nCols - 1)];
                currRow[col] += std::min(left, std::min(middle, right));
            }
        }
        return dst;
    }
    cv::Mat horizontalCumulativeMat(const cv::Mat &inputEnergyMat) {
    /** @brief rotates, sums from top to bottom in the DP manner:
    * M(i,j) = e(i,j) + min(M(i-1,j-1), M(i-1,j), M(i-1, j+1)) by using verticalCumulativeMat(...),
    * rotates back
    @param inputEnergyMat: CV_32SC1 input array
    @return rotated: CV_32SC1 output array containing cumulative sum for horizontal seams searching
    */
        cv::Mat rotated;
        cv::rotate(inputEnergyMat, rotated, cv::ROTATE_90_CLOCKWISE);
        rotated = verticalCumulativeMat(rotated);
        cv::rotate(rotated, rotated, cv::ROTATE_90_COUNTERCLOCKWISE);
        return rotated;
    }

    std::vector<std::pair<int, int>> findVerticalSeam(const cv::Mat &eMat) {
    /** @brief finds the minimum value at the last row, takes its index, searches for another seams
     * from the bottom to the top, at the end reverses order of pixels
    @param eMat: CV_32SC1 input array of cumulative energy
    @return seam: vector of std::pairs<row,col>; seam's path
    */
        auto lastRow = eMat.row(eMat.rows - 1);
        std::vector<std::pair<int, int>> seam;
        int minIndex = getMinimumIndex<int>(lastRow);
        seam.emplace_back(eMat.rows - 1, minIndex); //seam's beginning

        for (auto i = eMat.rows - 2; i >= 0; i--) {
            //find new seam based on a previous one
            seam.push_back(validPair(eMat, seam.back()));
        }
        //changing to the top to bottom order
        std::reverse(seam.begin(), seam.end());
        return seam;
    }

    std::pair<int, int> validPair(const cv::Mat &eMat, std::pair<int, int> prevPair) {
        int pRow = prevPair.first;
        int pCol = prevPair.second;
        int cRow = pRow - 1;
        int tCols = eMat.cols;

        if (cRow < 0) {
            throw std::runtime_error("Invalid seam pair");
        }
        int minCol = pCol;
        int minVal = eMat.at<int>(cRow, pCol);
        if (pCol > 0 && eMat.at<int>(cRow, pCol - 1) < minVal) {
            minCol = pCol - 1;
        }
        if (pCol < tCols - 1 && eMat.at<int>(cRow, pCol + 1) < minVal) {
            minCol = pCol + 1;
        }
        return std::make_pair(cRow, minCol);
    }

    std::vector<std::pair<int, int>> findHorizontalSeam(const cv::Mat &eMat) {
    /** @brief rotates energy matrix and uses findVerticalSeam(...) to find a horizontal seam
    @param eMat: CV_32SC1 input array of cumulative energy
    @return seam: vector of std::pairs<row,col>; seam's path
    */
        cv::Mat rotated;
        std::vector<std::pair<int, int>> seam;
        cv::rotate(eMat, rotated, cv::ROTATE_90_CLOCKWISE);
        seam = findVerticalSeam(rotated);
        return seam;
    }
    std::vector<std::pair<int, int>> rotSeamCntClk(const std::vector<std::pair<int, int>> &seam, const int &colsNo){
        auto rotatedSeam = seam;
        for (auto &p : rotatedSeam) {
            int pRow = p.first;
            int pCol = p.second;
            p.second = pRow;
            p.first = colsNo - 1 - pCol;
        }
        return rotatedSeam;
    }

    cv::Mat seamCarving(const cv::Mat &image, const cv::Size &out_size) {
        (void) out_size;
        return image;
    }
}
