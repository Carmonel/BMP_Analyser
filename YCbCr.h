#ifndef BMP_ANALYSER_YCBCR_H
#define BMP_ANALYSER_YCBCR_H

#include <vector>

#include "pixels.h"
#include "correlation.h"
#include "PSNR.h"

std::vector<YCbCrPixel> RGBtoYCbCr(const std::vector<RGBPixel>& pixels, int H, int W){
    std::vector<YCbCrPixel> result(H * W, YCbCrPixel());
    for (int i = 0; i < pixels.size(); i++){
        unsigned char Y = clipping(0.299*pixels[i].R + 0.587*pixels[i].G + 0.114*pixels[i].B);
        unsigned char Cb = clipping(0.5643*(pixels[i].B - Y) + 128);
        unsigned char Cr = clipping(0.7132*(pixels[i].R - Y) + 128);
        result[i] = YCbCrPixel(Y, Cb, Cr);
    }

    return result;
}

std::vector<RGBPixel> reworkRGB(std::vector<YCbCrPixel>& Y, int H, int W){
    std::vector<RGBPixel> newPixels(H * W, RGBPixel(0, 0, 0));
    for (int i = 0; i < H * W; i++){
        unsigned char R = clipping(Y[i].Y - 0.714*(Y[i].Cr - 128) - 0.334*(Y[i].Cb - 128));
        unsigned char G = clipping(Y[i].Y + 1.402*(Y[i].Cr - 128));
        unsigned char B = clipping(Y[i].Y + 1.772*(Y[i].Cb - 128));
        newPixels[i] = RGBPixel(R, G, B);
    }

    return newPixels;
}

void imageToYCbCrChannelsAutocorrelation(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, std::vector<YCbCrPixel>& pixels, int H, int W, const std::string& outputPath){
    std::cout << "!! If imageToYCbCrChannelsAutocorrelation() unable to open file create folder 'AC' in output directory !!" << std::endl;
    std::vector<unsigned char> channel(H * W);

    // Y autocorrelation
    for (int i = 0; i < H * W; i++) channel[i] = pixels[i].Y;
    autocorrelationEveryY(channel, H, W, outputPath + "autocorrelationY.txt");

    // Cb autocorrelation
    for (int i = 0; i < H * W; i++) channel[i] = pixels[i].Cb;
    autocorrelationEveryY(channel, H, W, outputPath + "autocorrelationCb.txt");

    // Cr autocorrelation
    for (int i = 0; i < H * W; i++) channel[i] = pixels[i].Cr;
    autocorrelationEveryY(channel, H, W, outputPath + "autocorrelationCr.txt");

    channel.clear();
}

void reworkRGBandCalculatePSNR(std::vector<YCbCrPixel>& Y, const std::vector<RGBPixel>& pixels, int H, int W){
    std::vector<RGBPixel> newPixels = reworkRGB(Y, H, W);

    std::vector<unsigned char> a(H * W, 0);
    std::vector<unsigned char> b(H * W, 0);

    // R
    for (int i = 0; i < H * W; i++){
        a[i] = pixels[i].R;
        b[i] = newPixels[i].R;
    }
    std::cout << "RGB -> YCbCr -> RGB, R correlation = " << PSNR(a, b) << std::endl;
    // G
    for (int i = 0; i < H * W; i++){
        a[i] = pixels[i].G;
        b[i] = newPixels[i].G;
    }
    std::cout << "RGB -> YCbCr -> RGB, G correlation = " << PSNR(a, b) << std::endl;
    // B
    for (int i = 0; i < H * W; i++){
        a[i] = pixels[i].B;
        b[i] = newPixels[i].B;
    }
    std::cout << "RGB -> YCbCr -> RGB, B correlation = " << PSNR(a, b) << std::endl;

    a.clear();
    b.clear();
    newPixels.clear();
}

void saveYCbCrImages(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, std::vector<YCbCrPixel>& pixels, int H, int W, const std::string& outputPath){
    // Y
    std::ofstream file(outputPath + "imageY.bmp", std::ios::binary | std::ios::out);
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    for (int i = 0; i < H * W; i++){
        file.write(reinterpret_cast<char*>(&pixels[i].Y), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&pixels[i].Y), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&pixels[i].Y), sizeof(unsigned char));
    }
    file.close();
    std::cout << outputPath + "imageY.bmp created." << std::endl;

    // Cb
    file = std::ofstream(outputPath + "imageCb.bmp", std::ios::binary | std::ios::out);
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    for (int i = 0; i < H * W; i++){
        file.write(reinterpret_cast<char*>(&pixels[i].Cb), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&pixels[i].Cb), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&pixels[i].Cb), sizeof(unsigned char));
    }
    file.close();
    std::cout << outputPath + "imageCb.bmp created." << std::endl;

    // Cr
    file = std::ofstream(outputPath + "imageCr.bmp", std::ios::binary | std::ios::out);
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    for (int i = 0; i < H * W; i++){
        file.write(reinterpret_cast<char*>(&pixels[i].Cr), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&pixels[i].Cr), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&pixels[i].Cr), sizeof(unsigned char));
    }
    file.close();
    std::cout << outputPath + "imageCr.bmp created." << std::endl;
}

void YCbCrCorrelation(const std::vector<YCbCrPixel>& pixels, int H, int W){
    std::vector<unsigned char> c1(H * W), c2(H * W);

    // Y - Cb
    for (int i = 0; i < H * W; i++){
        c1[i] = pixels[i].Y;
        c2[i] = pixels[i].Cb;
    }
    std::cout << "Y - Cb correlation: " << correlation(c1, c2) << std::endl;

    // Y - Cr
    for (int i = 0; i < H * W; i++){
        c1[i] = pixels[i].Y;
        c2[i] = pixels[i].Cr;
    }
    std::cout << "Y - Cr correlation: " << correlation(c1, c2) << std::endl;

    // Cb - Cr
    for (int i = 0; i < H * W; i++){
        c1[i] = pixels[i].Cb;
        c2[i] = pixels[i].Cr;
    }
    std::cout << "Cb - Cr correlation: " << correlation(c1, c2) << std::endl;

    c1.clear();
    c2.clear();
}

#endif //BMP_ANALYSER_YCBCR_H
