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
#include "DPCM.h"
#include "subImages.h"

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
    RGBChannelsCorrelation(pixels, H, W);
    // May return 0xC0000005, use debugger
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "| !! imageToChannelsAutocorrelation() may return 0xC0000005. Use debugger for results of func !! |" << std::endl;
    std::cout << "|                                       Try run func? y/n                                        |" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
    char ch;
    while ((ch != 'y') && (ch != 'n')){
        std::cin >> ch;
    }
    if (ch == 'y') imageToRGBChannelsAutocorrelation(fileHeader, infoHeader, pixels, H, W, outputPath + "AC\\");

    ///
    /// RGBtoYCbCr (5 - 7)
    ///
    std::vector<YCbCrPixel> YCbCrPixels = RGBtoYCbCr(pixels, H, W);
    YCbCrCorrelation(YCbCrPixels, H, W);
    saveYCbCrImages(fileHeader, infoHeader, YCbCrPixels, H, W, outputPath);
    reworkRGBandCalculatePSNR(YCbCrPixels, pixels, H, W);
    std::cout << std::endl;

    if (ch == 'y') imageToYCbCrChannelsAutocorrelation(fileHeader, infoHeader, YCbCrPixels, H, W, outputPath + "AC\\");
    std::cout << std::endl;

    ///
    /// decimation (8 - 11)
    ///
    decimation2(YCbCrPixels, pixels, H, W);
    std::cout << std::endl;
    decimation4(YCbCrPixels, pixels, H, W);
    std::cout << std::endl;

    ///
    /// Histograms (12)
    ///
    printRGB(pixels, H, W, outputPath);
    printYCbCr(YCbCrPixels, H, W, outputPath);
    std::cout << std::endl;

    ///
    /// Entropy (13)
    ///
    entropyRGB(pixels, H, W);
    entropyYCbCr(YCbCrPixels, H, W);
    std::cout << std::endl;

    ///
    /// DPCM (14 - 16)
    ///
    DPCMforRGB(pixels, H, W, outputPath + "DPCM\\");
    std::cout << std::endl;
    DPCMforYCbCr(YCbCrPixels, H, W, outputPath + "DPCM\\");
    std::cout << std::endl;

    ///
    /// 17
    ///
    makeSubImagesAndCalculateCorrelation(fileHeader, infoHeader, YCbCrPixels, H, W, outputPath + "Sub\\");

    pixels.clear();
    YCbCrPixels.clear();
}
