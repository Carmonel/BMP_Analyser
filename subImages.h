#ifndef BMP_ANALYSER_SUBIMAGES_H
#define BMP_ANALYSER_SUBIMAGES_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Windows.h"
#include "pixels.h"
#include "correlation.h"

void makeFile(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, std::vector<unsigned char>& Y, int H, int W, const std::string& outputPath){
    std::ofstream file(outputPath, std::ios::binary | std::ios::out);
    if (!file.is_open()){
        std::cerr << "makeSubImagesAndCalculateCorrelation(): cannot open file!" << std::endl;
        exit(-1);
    }
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    for (int i = 0; i < H * W; i++){
        file.write(reinterpret_cast<char*>(&Y[i]), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&Y[i]), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&Y[i]), sizeof(unsigned char));
    }
    file.close();
}

void makeSubImagesAndCalculateCorrelation(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, const std::vector<YCbCrPixel>& pixels, int H, int W, const std::string& outputPath){
    std::vector<unsigned char> Y00, Y01, Y10, Y11;
    for (int x = 0; x < H; x += 2){
        for (int y = 0; y < W; y += 2){
            Y00.push_back(pixels[x * W + y].Y);
            Y01.push_back(pixels[x * W + y + 1].Y);
            Y10.push_back(pixels[(x + 1) * W + y].Y);
            Y11.push_back(pixels[(x + 1) * W + y + 1].Y);
        }
    }
    infoHeader.biWidth /= 2;
    infoHeader.biHeight /= 2;
    infoHeader.biSizeImage = infoHeader.biWidth * infoHeader.biHeight;
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (H/2)*(W/2);

    makeFile(fileHeader, infoHeader, Y00, H / 2 , W / 2, outputPath + "Y00.bmp");
    std::cout << outputPath + "Y00.bmp created." << std::endl;
    makeFile(fileHeader, infoHeader, Y01, H / 2, W / 2, outputPath + "Y01.bmp");
    std::cout << outputPath + "Y01.bmp created." << std::endl;
    makeFile(fileHeader, infoHeader, Y10, H / 2, W / 2, outputPath + "Y10.bmp");
    std::cout << outputPath + "Y10.bmp created." << std::endl;
    makeFile(fileHeader, infoHeader, Y11, H / 2, W / 2, outputPath + "Y11.bmp");
    std::cout << outputPath + "Y11.bmp created." << std::endl;

    std::cout << "Y00 - Y01 correlation: " << correlation(Y00, Y01) << std::endl;
    std::cout << "Y00 - Y10 correlation: " << correlation(Y00, Y10) << std::endl;
    std::cout << "Y00 - Y11 correlation: " << correlation(Y00, Y11) << std::endl;
    std::cout << "Y01 - Y10 correlation: " << correlation(Y01, Y10) << std::endl;
    std::cout << "Y01 - Y11 correlation: " << correlation(Y01, Y11) << std::endl;
    std::cout << "Y10 - Y11 correlation: " << correlation(Y10, Y11) << std::endl;
}

#endif //BMP_ANALYSER_SUBIMAGES_H
