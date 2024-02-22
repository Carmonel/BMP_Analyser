cmake_minimum_required(VERSION 3.27)
project(BMP_Analyser)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

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

