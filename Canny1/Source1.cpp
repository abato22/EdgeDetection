/*#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void NoiseReduction(const Mat& gray_image, Mat& blurred_image, int kernel_size, double sigma) {
    GaussianBlur(gray_image, blurred_image, Size(kernel_size, kernel_size), sigma);
}

void Gradient(const Mat& blurred_image, Mat& gradient_magnitude, Mat& gradient_orientation) {

    Mat sobelx, sobely;
    Sobel(blurred_image, sobelx, CV_32F, 1, 0);
    Sobel(blurred_image, sobely, CV_32F, 0, 1);

    Mat sobx, soby;
    convertScaleAbs(sobelx, sobx);
    convertScaleAbs(sobely, soby);

    Mat grad;
    addWeighted(sobx, 0.5, soby, 0.5, 0, grad);

    namedWindow("Sobelx Image", WINDOW_NORMAL);
    resizeWindow("Sobelx Image", 700, 350);
    imshow("Sobelx Image", sobx);

    namedWindow("Sobely Image", WINDOW_NORMAL);
    resizeWindow("Sobely Image", 700, 350);
    imshow("Sobely Image", soby);

    namedWindow("Grad Image", WINDOW_NORMAL);
    resizeWindow("Grad Image", 700, 350);
    imshow("Grad Image", grad);

    waitKey(0);

    cartToPolar(sobelx, sobely, gradient_magnitude, gradient_orientation, true);
}

void nonMaxSuppression(const Mat& gradient_magnitude, const Mat& gradient_orientation, Mat& non_max_sup_image) {
    int rows = gradient_magnitude.rows;
    int cols = gradient_magnitude.cols;

    non_max_sup_image = Mat::zeros(rows, cols, CV_32F);

    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            float magnitude = gradient_magnitude.at<float>(i, j);
            float direction = gradient_orientation.at<float>(i, j);

            float q = 0, r = 0;

            if ((direction >= 0 && direction < 22.5) || (direction >= 157.5 && direction <= 180)) {
                q = gradient_magnitude.at<float>(i, j + 1);
                r = gradient_magnitude.at<float>(i, j - 1);
            }
            else if (direction >= 22.5 && direction < 67.5) {
                q = gradient_magnitude.at<float>(i + 1, j - 1);
                r = gradient_magnitude.at<float>(i - 1, j + 1);
            }
            else if (direction >= 67.5 && direction < 112.5) {
                q = gradient_magnitude.at<float>(i + 1, j);
                r = gradient_magnitude.at<float>(i - 1, j);
            }
            else if (direction >= 112.5 && direction < 157.5) {
                q = gradient_magnitude.at<float>(i - 1, j - 1);
                r = gradient_magnitude.at<float>(i + 1, j + 1);
            }

            if (magnitude >= q && magnitude >= r) {
                non_max_sup_image.at<float>(i, j) = magnitude;
            }
        }
    }
}

void doubleThreshold(const Mat& non_max_sup_image, Mat& thresholded_image, float low_threshold, float high_threshold) {
    int rows = non_max_sup_image.rows;
    int cols = non_max_sup_image.cols;

    thresholded_image = Mat::zeros(rows, cols, CV_8U);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float pixel = non_max_sup_image.at<float>(i, j);
            if (pixel >= high_threshold) {
                thresholded_image.at<uchar>(i, j) = 255;
            }
            else if (pixel >= low_threshold) {
                thresholded_image.at<uchar>(i, j) = 128;
            }
        }
    }
}

void edgeTrackingByHysteresis(const Mat& thresholded_image, Mat& edge_tracked_image) {
    int rows = thresholded_image.rows;
    int cols = thresholded_image.cols;

    edge_tracked_image = thresholded_image.clone();

    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            if (thresholded_image.at<uchar>(i, j) == 128) {
                if (thresholded_image.at<uchar>(i + 1, j - 1) == 255 || thresholded_image.at<uchar>(i + 1, j) == 255 ||
                    thresholded_image.at<uchar>(i + 1, j + 1) == 255 || thresholded_image.at<uchar>(i, j - 1) == 255 ||
                    thresholded_image.at<uchar>(i, j + 1) == 255 || thresholded_image.at<uchar>(i - 1, j - 1) == 255 ||
                    thresholded_image.at<uchar>(i - 1, j) == 255 || thresholded_image.at<uchar>(i - 1, j + 1) == 255) {
                    edge_tracked_image.at<uchar>(i, j) = 255;
                }
                else {
                    edge_tracked_image.at<uchar>(i, j) = 0;
                }
            }
        }
    }
}

int main() {
    Mat original_image = imread("C:/6. Semestar/Završni rad/Pictures/licenseplate.jpg");

    if (original_image.empty()) {
        cout << "Could not open or find the image!" << endl;
        return -1;
    }

    Mat gray_image;
    cvtColor(original_image, gray_image, COLOR_BGR2GRAY);

    Mat blurred_image;
    NoiseReduction(gray_image, blurred_image, 9, 1.6);

    Mat gradient_magnitude, gradient_orientation;
    Gradient(blurred_image, gradient_magnitude, gradient_orientation);

    Mat non_max_sup_image;
    nonMaxSuppression(gradient_magnitude, gradient_orientation, non_max_sup_image);

    Mat thresholded_image;
    doubleThreshold(non_max_sup_image, thresholded_image, 50, 150);

    Mat edge_tracked_image;
    edgeTrackingByHysteresis(thresholded_image, edge_tracked_image);

    namedWindow("Original Image", WINDOW_NORMAL);
    resizeWindow("Original Image", 700, 350);
    imshow("Original Image", original_image);

    namedWindow("Gray Image", WINDOW_NORMAL);
    resizeWindow("Gray Image", 700, 350);
    imshow("Gray Image", gray_image);

    namedWindow("Blurred Image", WINDOW_NORMAL);
    resizeWindow("Blurred Image", 700, 350);
    imshow("Blurred Image", blurred_image);

    namedWindow("Gradient Magnitude Image", WINDOW_NORMAL);
    resizeWindow("Gradient Magnitude Image", 700, 350);
    imshow("Gradient Magnitude Image", gradient_magnitude);

    namedWindow("Gradient Orientation Image", WINDOW_NORMAL);
    resizeWindow("Gradient Orientation Image", 700, 350);
    imshow("Gradient Orientation Image", gradient_orientation);

    namedWindow("Non Max Suppression Image", WINDOW_NORMAL);
    resizeWindow("Non Max Suppression Image", 700, 350);
    imshow("Non Max Suppression Image", non_max_sup_image);

    namedWindow("Thresholded Image", WINDOW_NORMAL);
    resizeWindow("Thresholded Image", 700, 350);
    imshow("Thresholded Image", thresholded_image);

    namedWindow("Edge Tracked Image", WINDOW_NORMAL);
    resizeWindow("Edge Tracked Image", 700, 350);
    imshow("Edge Tracked Image", edge_tracked_image);

    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/NonMaxSuppression.jpg", non_max_sup_image);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/GradientMagnitude.jpg", gradient_magnitude);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/GradientOrientation.jpg", gradient_orientation);

    waitKey(0);

    return 0;
}*/