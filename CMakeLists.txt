cmake_minimum_required(VERSION 3.10)
project(KANvoluCLion)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "P:/opencv/build/include")

# Find OpenCV package
find_package(OpenCV REQUIRED)



# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add your executable
add_executable(KANvoluCLion main.cpp
        mcuConv.cpp
        mcuConv.h)

# Link against OpenCV libraries
target_link_libraries(KANvoluCLion ${OpenCV_LIBS})