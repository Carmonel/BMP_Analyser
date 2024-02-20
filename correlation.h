#ifndef BMP_ANALYSER_CORRELATION_H
#define BMP_ANALYSER_CORRELATION_H

#include <iostream>
#include <fstream>
#include <string>
#include "Windows.h"
#include <vector>
#include <cmath>

#include "pixels.h"

double correlation(std::vector<unsigned char> a, std::vector<unsigned char> b);

std::map<int, double> autocorrelation(std::vector<unsigned char>& src, int H, int W, int y){
    std::map<int, double> result;

    #pragma omp parallel for
    for (int x = 0; x < W / 4; x++){
        std::vector<unsigned char> a = std::vector<unsigned char>();
        std::vector<unsigned char> b = std::vector<unsigned char>();
        for (int i = 1; i < H - y; i++){
            for (int j = 1; j < W - x; j++){
                a.push_back(src[i * W + j]);
            }
        }
        for (int m = y + 1; m < H; m++){
            for (int n = x + 1; n < W; n++){
                b.push_back(src[m * W + n]);
            }
        }
        result.insert(std::pair<int, double>(x, correlation(a, b)));
        a.clear();
        b.clear();
    }

    return result;
}

double correlation(std::vector<unsigned char> a, std::vector<unsigned char> b){
    int size;
    if (a.size() != b.size()){
        std::cout << "!! correlation(): a.size() != b.size() !!" << std::endl;
        size = std::min(a.size(), b.size());
    } else size = a.size();

    // МО
    double m_a = 0, m_b = 0;
    for (int i = 0; i < size; i++){
        m_a += a[i];
        m_b += b[i];
    }
    m_a /= size;
    m_b /= size;

    // СКО
    double b_a = 0, b_b = 0;
    for (int i = 0; i < size; i++){
        b_a += (a[i] - m_a) * (a[i] - m_a);
        b_b += (b[i] - m_b) * (b[i] - m_b);
    }
    b_a /= size - 1;
    b_b /= size - 1;
    b_a = sqrt(b_a);
    b_b = sqrt(b_b);

    // correlation
    double result = 0;
    for (int i = 0; i < size; i++){
        result += (a[i] - m_a) * (b[i] - m_b);
    }
    result /= size;
    result /= b_a * b_b;

    if ((result > 1.0) || (result < -1.0)) std::cout << "!! correlation(): result != [-1; 1] !!" << std::endl;

    return result;
}

#endif //BMP_ANALYSER_CORRELATION_H
