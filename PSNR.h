#ifndef BMP_ANALYSER_PSNR_H
#define BMP_ANALYSER_PSNR_H

#include <vector>
#include <iostream>
#include <cmath>

double PSNR(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b){
    double result;
    int size;
    if (a.size() != b.size()){
        std::cout << "!! PSNR(): a.size() != b.size() !! " << std::endl;
        size = std::min(a.size(), b.size());
    }else size = a.size();

    for (int i = 0; i < size; i++){
        result += (a[i] - b[i]) * (a[i] - b[i]);
    }
    result = size * (pow(2, 8) - 1) * (pow(2, 8) - 1) / result;

    return 10*log10(result);
}

#endif //BMP_ANALYSER_PSNR_H
