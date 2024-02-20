#ifndef BMP_ANALYSER_HISTOGRAM_H
#define BMP_ANALYSER_HISTOGRAM_H

#include <vector>
#include <iostream>
#include <fstream>
#include "pixels.h"

template<class T>
void printHistogram(std::vector<T> data, const std::string& path){
    std::ofstream file(path);
    if (!file.is_open()){
        std::cout << "file wasn't open!";
        exit(-1);
    }

    for (int i = 0; i < data.size(); i++){
        file << i << "=" << (int) data[i] << std::endl;
    }

    std::cout << path + " created." << std::endl;

    file.close();
}

void printRGB(const std::vector<RGBPixel>& a, int H, int W, const std::string& outputPath){
    std::vector<unsigned char> freqR(256, 0);
    std::vector<unsigned char> freqG(256, 0);
    std::vector<unsigned char> freqB(256, 0);
    for (int i = 0; i < H * W; i++){
        freqR[a[i].R]++;
        freqG[a[i].G]++;
        freqB[a[i].B]++;
    }

    printHistogram(freqR, outputPath + "freqR.txt");
    printHistogram(freqG, outputPath + "freqG.txt");
    printHistogram(freqB, outputPath + "freqB.txt");

    freqR.clear();
    freqB.clear();
    freqG.clear();
}

void printYCbCr(const std::vector<YCbCrPixel>& a, int H, int W, const std::string& outputPath){
    std::vector<unsigned char> freqY(256, 0);
    std::vector<unsigned char> freqCb(256, 0);
    std::vector<unsigned char> freqCr(256, 0);
    for (int i = 0; i < H * W; i++){
        freqY[a[i].Y]++;
        freqCb[a[i].Cb]++;
        freqCr[a[i].Cr]++;
    }

    printHistogram(freqY, outputPath + "freqY.txt");
    printHistogram(freqCb, outputPath + "freqCb.txt");
    printHistogram(freqCr, outputPath + "freqCr.txt");

    freqY.clear();
    freqCr.clear();
    freqCb.clear();
}

#endif //BMP_ANALYSER_HISTOGRAM_H
