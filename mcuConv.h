// //
// // Created by apumu on 9/14/2024.
// //
#ifndef MCUCONV_H
#define MCUCONV_H
#include <vector>
//
// This intends to be a library for microcontrollers to access, in order to run
// tile-based convolution and other CV/NN algorithms I wanna play around with, optimally
// on any microcontroller (starting with an STM32).
//
// We're starting here with a basic blurring convolution operation, with a non-learnable kernel.
// [sorry ML nerds no deep learning just yet :( ]
// I'm gonna try to scale up to an object classifier, and you can check out the other planned
// projects, but for now one step at a time, contemporary CV first,
// maybe an outline creater using a pde kernel filter?
//
// We want this to run onboard an MCU, so that the memory is utilized properly.
// but we're not prototyping on an MCU yet.
// for our simulation now, we will abstract the MCU direct memory access(DMA).
//

class mcuConv {
    public:
        mcuConv(uint16_t inputWidth, uint16_t inputHeight, uint8_t inputChannels,
                uint16_t kernelSize, uint8_t kernelCount, uint8_t bytesPerPixel);

        ~mcuConv();

        void setInput(const uint8_t* input);
        void setKernels(const float* kernels);
        void tiledConv();
        const uint8_t* getOutput();

        void configureDMA();

    class Kernel {
    protected:
        int size;
        float* data;

    public:
        Kernel(int size) : size(size) {
            data = new float[size * size];
        }

        virtual ~Kernel() {
            delete[] data;
        }

        int getSize() const { return size; }
        float* getData() const { return data; }
    };

    class BlurKernel : public Kernel {
    public:
        BlurKernel(int size, float blurFactor) : Kernel(size) {
            float sum = 0.0f;
            for (int i = 0; i < size * size; i++) {
                data[i] = 1.0f;
                sum += data[i];
            }
            float normalizer = 1.0f / (sum * blurFactor);
            for (int i = 0; i < size * size; i++) {
                data[i] *= normalizer;
            }
        }
    };
    void convolve(const unsigned char* input, unsigned char* output, int width, int height, const Kernel& kernel);



    private:

        struct DMAConfig {
            uint32_t bus_width;
            uint32_t mem_alignment;
            uint32_t burst_size;
            uint32_t fifo_threshold;
            uint32_t periph_data_size;
            uint32_t mem_data_size;
            uint32_t transfer_mode;
            uint32_t priority;
        };

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_channels;
        uint32_t m_kernelSize;
        uint32_t m_kernelCount;
        uint32_t m_bytesPerPixel;
        std::vector<uint8_t> m_inputBuffer;
        std::vector<uint8_t> m_outputBuffer;
        std::vector<float> m_kernels;
        DMAConfig m_dmaConfig;

};

#endif //MCUCONV_H

