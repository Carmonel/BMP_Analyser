#ifndef BMP_ANALYSER_IMAGETORGBCHANNELS_H
#define BMP_ANALYSER_IMAGETORGBCHANNELS_H

#include <iostream>
#include <fstream>
#include <string>
#include "Windows.h"
#include <vector>
#include <filesystem>
#include <map>

#include "pixels.h"
#include "correlation.h"

void autocorrelationEveryY(std::vector<unsigned char>& channel, int H, int W, const std::string& outputPath){
    std::ofstream file(outputPath);

    std::map<int, double> ac1 = autocorrelation(channel, H, W, -10);
    std::map<int, double> ac2 = autocorrelation(channel, H, W, -5);
    std::map<int, double> ac3 = autocorrelation(channel, H, W, 0);
    std::map<int, double> ac4 = autocorrelation(channel, H, W, 5);
    std::map<int, double> ac5 = autocorrelation(channel, H, W, 10);
    for (int i = -(W / 4 - 1); i < W / 4; i++){
        file << i << "=" << ac1.at(i) << "=" << ac2.at(i) << "=" << ac3.at(i) << "=" << ac4.at(i) << "=" << ac4.at(i) << std::endl;
    }
    ac1.clear();
    ac2.clear();
    ac3.clear();
    ac4.clear();
    ac5.clear();
    std::cout << outputPath + " created." << std::endl;
    file.close();
}

void imageToRGBChannelsAutocorrelation(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, std::vector<RGBPixel>& pixels, int H, int W, const std::string& outputPath){
    std::cout << "!! If imageToRGBChannelsAutocorrelation() unable to open file create folder 'AC' in output directory !!" << std::endl;
    std::vector<unsigned char> channel(H * W);

    // R autocorrelation
    for (int i = 0; i < H * W; i++) channel[i] = pixels[i].R;
    autocorrelationEveryY(channel, H, W, outputPath + "autocorrelationRed.txt");

    // G autocorrelation
    for (int i = 0; i < H * W; i++) channel[i] = pixels[i].G;
    autocorrelationEveryY(channel, H, W, outputPath + "autocorrelationGreen.txt");

    // B autocorrelation
    for (int i = 0; i < H * W; i++) channel[i] = pixels[i].B;
    autocorrelationEveryY(channel, H, W, outputPath + "autocorrelationBlue.txt");

    channel.clear();
}

void RGBChannelsCorrelation(std::vector<RGBPixel>& pixels, int H, int W){
    std::vector<unsigned char> c1(H * W), c2(H * W);

    // BLUE - GREEN
    for (int i = 0; i < H * W; i++){
        c1[i] = pixels[i].B;
        c2[i] = pixels[i].G;
    }
    std::cout << "RED - GREEN correlation: " << correlation(c1, c2) << std::endl;

    // GREEN - RED
    for (int i = 0; i < H * W; i++){
        c1[i] = pixels[i].G;
        c2[i] = pixels[i].R;
    }
    std::cout << "RED - GREEN correlation: " << correlation(c1, c2) << std::endl;

    // BLUE - RED
    for (int i = 0; i < H * W; i++){
        c1[i] = pixels[i].B;
        c2[i] = pixels[i].R;
    }
    std::cout << "RED - GREEN correlation: " << correlation(c1, c2) << std::endl;

    c1.clear();
    c2.clear();
}

void imageToRGBChannels(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, std::vector<RGBPixel>& pixels, int H, int W, const std::string& outputPath){
    unsigned char null = 0;

    // BLUE channel
    std::ofstream bImage(outputPath + "BLUE.bmp", std::ios::out | std::ios::binary);
    if (!bImage.is_open()){
        std::cout << "bImage wasn't open!";
        exit(-1);
    }
    bImage.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    bImage.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    for (int i = 0; i < H * W; i++){
        bImage.write(reinterpret_cast<char*>(&pixels[i].B), sizeof(unsigned char));
        bImage.write(reinterpret_cast<char*>(&null), sizeof(unsigned char));
        bImage.write(reinterpret_cast<char*>(&null), sizeof(unsigned char));
    }
    bImage.close();
    std::cout << "Blue channel created." << std::endl;

    // GREEN channel
    std::ofstream gImage(outputPath + "GREEN.bmp", std::ios::out | std::ios::binary);
    if (!gImage.is_open()){
        std::cout << "gImage wasn't open!";
        exit(-1);
    }
    gImage.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    gImage.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    for (int i = 0; i < H * W; i++){
        gImage.write(reinterpret_cast<char*>(&null), sizeof(unsigned char));
        gImage.write(reinterpret_cast<char*>(&pixels[i].G), sizeof(unsigned char));
        gImage.write(reinterpret_cast<char*>(&null), sizeof(unsigned char));
    }
    gImage.close();
    std::cout << "Green channel created." << std::endl;

    // RED channel
    std::ofstream rImage(outputPath + "RED.bmp", std::ios::out | std::ios::binary);
    if (!rImage.is_open()){
        std::cout << "rImage wasn't open!";
        exit(-1);
    }
    rImage.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    rImage.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    for (int i = 0; i < H * W; i++){
        rImage.write(reinterpret_cast<char*>(&null), sizeof(unsigned char));
        rImage.write(reinterpret_cast<char*>(&null), sizeof(unsigned char));
        rImage.write(reinterpret_cast<char*>(&pixels[i].R), sizeof(unsigned char));
    }
    rImage.close();
    std::cout << "Red channel created." << std::endl << std::endl;
}

#endif //BMP_ANALYSER_IMAGETORGBCHANNELS_H
