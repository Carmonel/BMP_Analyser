cmake_minimum_required(VERSION 3.27)
project(BMP_Analyser)

set(CMAKE_CXX_STANDARD 17)

add_executable(BMP_Analyser main.cpp
        pixels.h
        imageToRGBChannels.h
        correlation.h
        YCbCr.h
        PSNR.h
        decimation.h
        histogram.h
        entropy.h
        DPCM.h
        subImages.h)
