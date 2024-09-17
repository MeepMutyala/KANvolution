#include <opencv2/opencv.hpp>
#include "mcuConv.hpp"

int main() {
    cv::VideoCapture cam(0); // Open the default camera (usually the webcam)

    if (!cam.isOpened()) {
        std::cerr << "Error opening webcam" << std::endl;
        return -1;
    }

    cv::Mat frame;
    mcuConv convolver(640, 480, 3, 5, 1, 1); // Adjust parameters as needed
    mcuConv::BlurKernel blurKernel(5, 0.7f); // 5x5 kernel with blur factor
    mcuConv::Kernel sobelKernelX(3);
    mcuConv::Kernel sobelKernelY(3);

    // Initialize Sobel kernels
    float sobelDataX[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float sobelDataY[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    std::copy(sobelDataX, sobelDataX + 9, sobelKernelX.getData());
    std::copy(sobelDataY, sobelDataY + 9, sobelKernelY.getData());


    while (true) {
        cam >> frame; // capture a frame from the camera
        if (frame.empty()) break;
        int width = frame.cols;
        int height = frame.rows;

        // allocating memory for frame loading, and for computed products
        std::vector<unsigned char> inputFrame(width * height * 3);
        std::vector<unsigned char> outputBuffer(width * height * 3);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x);
                int index = (y * width + x) * 3;

                inputFrame[index] = pixel[0];     // Blue
                inputFrame[index + 1] = pixel[1]; // Green
                inputFrame[index + 2] = pixel[2]; // Red
            }
        }

        // display a window with the camera frame data
        cv::imshow("Webcam", frame);

        //convolve with the blur kernel
        convolver.convolve(inputFrame.data(), outputBuffer.data(), width, height, blurKernel, NULL, 1, 1);
        cv::Mat blurredFrame(height, width, CV_8UC3, outputBuffer.data());
        cv::imshow("Blurred Convolution", blurredFrame);

        // convert to b&w for edge detection
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

        // convolve with both gradient filters to produce outlines
        convolver.convolve(inputFrame.data(), outputBuffer.data(), width, height, sobelKernelX, sobelKernelY, 2, 1);
        cv::Mat outlinesFrame(height, width, CV_8UC3, outputBuffer.data());
        cv::imshow("Edge Convolution", outlinesFrame);

        // to break
        if (cv::waitKey(1) >= 0) break;
    }

    return 0;
}