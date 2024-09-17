# 🧠 Edge Vision: KANs, ViTs, and Convolutions on STM32 🚀
Hello and welcome! This is an edge computer vision project in it's early stages, still a major work in progress. The project scope, outline, and evaluation metrics are defined below, with a discussion at the end.
![blurEdgeConv](https://github.com/user-attachments/assets/7a3adb71-b8ea-437c-80a1-59ac9839e9ae)

## 🎯 Project Goals

We're building a microcontroller-friendly library for running tile-based convolution and other CV/NN algorithms, starting with the STM32 platform. I thought it'd be cool to see if I can make a library that can optimize performance across different microcontroller architectures.

## 🔮 Scope

1. Develop the learnable MLP-based kernel and it's convolution algorithm.
2. Implement memory tiling-based convolution for this MLP kernel based on simulated architecture.
3. Design and implement a [Kolmogorov-Arnold Network (KAN)](https://www.youtube.com/watch?v=xLnQtLpPH-Y&t=3s) based CNN 
4. Literature review for optimizing Vision Transformer (ViT) Direct Memory Access inference
5. Designing and implementing the ViT
6. Scaling model layers modularly up to an object classifier

## 🛠️ Project Components

### 1. Data Acquisition
We're working on loading camera buffer data onto the STM32 to manipulate raw pixel data. Currently, we're simulating the Direct Memory Access (DMA) configuration of the STM32.

### 2. Algorithm Implementation
We're programming these optimized algorithms from scratch in C++. It's gonna be a tough challenge, but will also deepen my understanding of these algorithms and how we can take advantage of different model architectures, both deep learning and embedded, to integrate them with each other more tightly.

## 🚀 Current Status

So far, we have a basic blurring convolution operation, a mixture of gradient convolutions (edge detection/outline drawing) with non-learnable kernels, and Direct Memory Access (DMA) simulated. Still have a ways to go, but these are important foundations for what's to come!

## 🔍 Current Focus

1. Reading raw frame data from a Logitech C270 webcam to the computer
2. Programming convolution and optimized algorithms for general use
3. Creating an abstraction layer for STM32 integration
4. Expanding compatibility to other MCUs

## 📊 Performance Metrics and Benchmarks

The point of this is to see how much we can optimize the efficiency, with regards to multiple factors. We'll measure"

1. **Inference Time**: How long it takes to process a single image.
2. **Frame Rate (FPS)**: # frames processed per second.
3. **Memory Usage**: RAM during inference and Flash for storage.
4. **Power Consumption**: not sure how to track this yet, but an important consideration.
5. **Accuracy**: optimizations.
6. **Model Size**: Comparing different architectures and quantization levels.
7. **Latency**: Time between input and output for real-time applications.
8. **CPU Utilization**: How much of the STM32/MCU's capacity we're using.
9. **Quantization Effects**: Comparing float vs fixed-point implementations.
10. **Memory Bandwidth**: Efficiency of data movement between memory and processor.

and at the end of the project we'll analyze tradeoffs between:
- Accuracy vs. Speed
- Model Size vs. Performance
- Power Consumption vs. Processing Capability

## 💡 Why do we care?

Making requests that have to be processed through the internet, a backend, GPU clusters configured through cloud servers, ends up introducing a lot of latency in inference. As a result, we want to call inference as close to our processor as possible. We can either route the memory quickly to whichever layer computes (cloud GPUs, onboard GPUs/CPUs, etc) our matrix, convolution, and other deep learning operations, and send the result back through the pipeline, or we can move our algorithm and memory calls as close to the onboard processor as possible. By running compute-intensive ML algorithms on a lower level on an embedded device, rather than going through multiple layers of abstraction, we can get the results we want much faster. I want to learn how to do this, cause it's fun to see how engineering architectures (deep learning and embedded) can fit together.
