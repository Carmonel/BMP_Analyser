#include <iostream>

#include <fstream>
#include <iostream>
#include "Windows.h"
#include <vector>

#include "pixels.h"
#include "imageToRGBChannels.h"
#include "YCbCr.h"
#include "decimation.h"
#include "histogram.h"
#include "entropy.h"

int main(int argc, char* argv[]) {
    if (argc != 3){
        std::cerr << "Argument count != 3!" << std::endl;
        std::cerr << "Using: -inputFile -outputDirectory" << std::endl;
        std::cerr << "Example: run.exe E:\\image\\image.bmp E:\\image\\" << std::endl;
        exit(1);
    }
    std::string inputPath = argv[1];
    std::string outputPath = argv[2];

    int H;
    int W;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    std::ifstream image(inputPath,std::ios::in | std::ios::binary);

    if (!image.is_open()){
        std::cerr << "Image wasn't open!";
        exit(-1);
    }

    image.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    image.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (infoHeader.biBitCount != 24) {
        std::cerr << "Image is not True Color 24 BMP!";
        exit(-1);
    }

    W = infoHeader.biWidth;
    H = infoHeader.biHeight;

    std::vector<RGBPixel> pixels(W * H, new RGBPixel(0, 0, 0));
    image.read(reinterpret_cast<char*>(pixels.data()), infoHeader.biSizeImage);
    image.close();
    std::cout << "File read." << std::endl << std::endl;


    ///
    /// Image to R, G, B channels (3)
    ///
    imageToRGBChannels(fileHeader, infoHeader, pixels, H, W, outputPath);

    ///
    /// Correlation & autocorrelation (4)
    ///
    imageToRGBChannelsCorrelation(fileHeader, infoHeader, pixels, H, W);
    //imageToRGBChannelsAutocorrelation(fileHeader, infoHeader, pixels, H, W, outputPath + "AC\\"); // May return 0xC0000005

    ///
    /// RGBtoYCbCr (5 - 7)
    ///
    std::vector<YCbCrPixel> YCbCrPixels = RGBtoYCbCr(pixels, H, W);
    YCbCrCorrelation(YCbCrPixels, H, W);
    saveYCbCrImages(fileHeader, infoHeader, YCbCrPixels, H, W, outputPath);
    reworkRGBandCalculatePSNR(YCbCrPixels, pixels, H, W);

    ///
    /// decimation (8-11)
    ///
    decimation2(YCbCrPixels, pixels, H, W);
    decimation4(YCbCrPixels, pixels, H, W);

    ///
    /// Histograms (12)
    ///
    printRGB(pixels, H, W, outputPath);
    printYCbCr(YCbCrPixels, H, W, outputPath);

    ///
    /// Entropy (13)
    ///
    entropyRGB(pixels, H, W);
    entropyYCbCr(YCbCrPixels, H, W);

    ///
    /// DPCM (15)
    ///
    

    pixels.clear();
    YCbCrPixels.clear();
}
