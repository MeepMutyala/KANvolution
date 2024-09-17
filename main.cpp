#include <opencv2/opencv.hpp>
#include "mcuConv.h"

int main() {
    cv::VideoCapture cam(0); // Open the default camera (usually the webcam)

    if (!cam.isOpened()) {
        std::cerr << "Error opening webcam" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cam >> frame; // capture a frame from the camera
        if (frame.empty()) break;

        // Access pixel values

        int width = frame.cols;
        int height = frame.rows;


        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cv::Vec3b& pixel = frame.at<cv::Vec3b>(y, x);

                uchar blue = pixel[0];
                uchar green = pixel[1];
                uchar red = pixel[2];

                // Process pixel values here
            }
        }

        cv::imshow("Webcam", frame);
        if (cv::waitKey(1) >= 0) break;
    }

    return 0;
}