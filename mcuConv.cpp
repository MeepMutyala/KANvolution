#include <cstdint>
#include "mcuConv.hpp"
#include <cmath>
// #include "sampleMCU.h" // Uncomment and replace with appropriate MCU header
#include <algorithm>

mcuConv::mcuConv(uint16_t inputWidth, uint16_t inputHeight, uint8_t inputChannels,
                 uint16_t kernelSize, uint8_t kernelCount, uint8_t bytesPerPixel)
    : m_width(inputWidth), m_height(inputHeight), m_channels(inputChannels),
      m_kernelSize(kernelSize), m_kernelCount(kernelCount), m_bytesPerPixel(bytesPerPixel)
{
        m_inputBuffer.resize(m_width * m_height * m_channels * m_bytesPerPixel);
        m_outputBuffer.resize(m_width * m_height * m_kernelCount * m_bytesPerPixel);
        m_kernels.resize(m_kernelSize * m_kernelSize * m_channels * m_kernelCount);

        // Initialize DMA configuration with default values
        m_dmaConfig = {
                .bus_width = 32,
                .mem_alignment = 4,
                .burst_size = 4,
                .fifo_threshold = 2,  // Half full
                .periph_data_size = 8,  // Assuming 8-bit data from camera
                .mem_data_size = 32,    // Storing in 32-bit words
                .transfer_mode = 1,     // Circular mode
                .priority = 2           // High priority
            };
}

mcuConv::~mcuConv()
{


}

// this function simply copies the data from the buffer to the "input"
void mcuConv::setInput(const uint8_t* input)
{
        std::copy(input, input + m_inputBuffer.size(), m_inputBuffer.begin());
}

//function for setting kernels??
void mcuConv::setKernels(const float* kernels)
{
        std::copy(kernels, kernels + m_kernels.size(), m_kernels.begin());
}



void mcuConv::convolve(const unsigned char* input, unsigned char* output, int width, int height, const Kernel& kernelX, const Kernel& kernelY, int numKernels = 1, int stride = 1) {
        int kernelSize = kernelX.getSize();
        int kernelRadius = kernelSize / 2;      // we use this to iterate through the rows and columns of kernel
        const float* kernelDataX = kernelX.getData();                                  // load x kernel or default kernel data
        const float* kernelDataY = (numKernels == 2) ? kernelY.getData() : nullptr;    // load y kernel data or none

        for (int y=0; y<height; y+=stride) {
                for (int x=0; x<width; x+=stride) {
                        // Initialize sums for horizontal and vertical gradients
                        float sumsX_R = 0, sumsX_G = 0, sumsX_B = 0;
                        float sumsY_R = 0, sumsY_G = 0, sumsY_B = 0;

                        for(int kernY = -kernelRadius; kernY <= kernelRadius; kernY++) {         // this iterates th
                                for(int kernX = -kernelRadius; kernX <= kernelRadius; kernX++) {
                                        // we're doing this, reusing edge pixel values, instead of padding
                                        // because we don't have to allocate more memory by adding padding this way.

                                        int pixelY = std::min(std::max(y + kernY, 0), height - 1);
                                        int pixelX = std::min(std::max(x + kernX, 0), width - 1);

                                        int pixelIndex = (pixelY * width + pixelX) * 3;
                                        float weightX = kernelDataX[(kernY + kernelRadius) * kernelSize + (kernX + kernelRadius)];

                                        // Calculate weighted sums for x kernel or single kernel
                                        sumsX_B += input[pixelIndex] * weightX;
                                        sumsX_G += input[pixelIndex + 1] * weightX;
                                        sumsX_R += input[pixelIndex + 2] * weightX;

                                        // this is for outlines, when we have two kernel filters, usually to capture gradients or changes in axis.
                                        // we have to compute these separately and combine via the Euclidean norm
                                        if (numKernels == 2 && kernelDataY) {
                                                float weightY = kernelDataY[(kernY + kernelRadius) * kernelSize + (kernX + kernelRadius)];
                                                sumsY_B += input[pixelIndex] * weightY;
                                                sumsY_G += input[pixelIndex + 1] * weightY;
                                                sumsY_R += input[pixelIndex + 2] * weightY;
                                        }
                                }
                        }
                        int outputIndex = (y * width + x) * 3; //pixel index we're setting

                        if (numKernels == 2) {
                                // Combine results for edge detection
                                output[outputIndex] = static_cast<unsigned char>(std::min(std::sqrt(sumsX_B * sumsX_B + sumsY_B * sumsY_B), 255.0f));
                                output[outputIndex + 1] = static_cast<unsigned char>(std::min(std::sqrt(sumsX_G * sumsX_G + sumsY_G * sumsY_G), 255.0f));
                                output[outputIndex + 2] = static_cast<unsigned char>(std::min(std::sqrt(sumsX_R * sumsX_R + sumsY_R * sumsY_R), 255.0f));
                        } else {
                                // Use single kernel result for blurring / others
                                output[outputIndex] = static_cast<unsigned char>(std::min(std::max(sumsX_B, 0.0f), 255.0f));
                                output[outputIndex + 1] = static_cast<unsigned char>(std::min(std::max(sumsX_G, 0.0f), 255.0f));
                                output[outputIndex + 2] = static_cast<unsigned char>(std::min(std::max(sumsX_R, 0.0f), 255.0f));
                        }
                }
        }

}



void mcuConv::tiledConv()
{
        //
}

const uint8_t* mcuConv::getOutput()
{
        return m_outputBuffer.data();
}

void mcuConv::configureDMA()
{
        // Implement DMA configuration logic here
        // Similar to the previous example, but using m_dmaConfig and class members
}