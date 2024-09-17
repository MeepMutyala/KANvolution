#include <cstdint>
#include "mcuConv.h"
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

void mcuConv::setInput(const uint8_t* input)
{
        std::copy(input, input + m_inputBuffer.size(), m_inputBuffer.begin());
}

void mcuConv::setKernels(const float* kernels)
{
        std::copy(kernels, kernels + m_kernels.size(), m_kernels.begin());
}



void mcuConv::convolve(const unsigned char* input, unsigned char* output, int width, int height, const Kernel& kernel) {

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