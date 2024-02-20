#ifndef BMP_ANALYSER_DECIMATION_H
#define BMP_ANALYSER_DECIMATION_H

#include "YCbCr.h"
#include "pixels.h"

void decimation4(const std::vector<YCbCrPixel>& YCbCrPixels, const std::vector<RGBPixel>& pixels, int H, int W){
    std::vector<YCbCrPixel> pixelsYCbCrEven(H * W, YCbCrPixel(0, 0, 0));
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            // Берем верхние строчки
            if (i % 4 != 0){
                pixelsYCbCrEven[i * W + j] = YCbCrPixels[(i - (i % 4)) * W + j];
                pixelsYCbCrEven[i].Y = YCbCrPixels[i].Y;
                continue;
            }
            // Берем слева
            if (j % 4 != 0){
                pixelsYCbCrEven[i * W + j] = YCbCrPixels[i * W - (j % 4)];
                pixelsYCbCrEven[i].Y = YCbCrPixels[i].Y;
                continue;
            }
            pixelsYCbCrEven[i * W + j] = YCbCrPixels[i * W + j];
        }
    }

    std::cout << "Decimation Cb, Cr /4 (Mid) started." << std::endl;
    // Децимация как среднее арифметическое
    std::vector<YCbCrPixel> pixelsYCbCrMid(H * W, YCbCrPixel(0, 0, 0));
    for (int i = 0; i < H; i = i + 4){
        for (int j = 0; j < W; j = j + 4){
            int tmpCb = 0;
            int tmpCr = 0;
            for (int k = 0; k < 4; k++){
                for (int l = 0; l < 4; l++){
                    tmpCb += YCbCrPixels[(i + k) * W + (j + l)].Cb;
                    tmpCr += YCbCrPixels[(i + k) * W + (j + l)].Cr;
                }
            }
            tmpCb /= 16;
            tmpCr /= 16;
            YCbCrPixel pixel(YCbCrPixels[i * W + j].Y, tmpCb, tmpCr);
            for (int k = 0; k < 4; k++){
                for (int l = 0; l < 4; l++){
                    pixelsYCbCrMid[(i + k) * W + (j + l)] = pixel;
                }
            }
        }
    }

    std::vector<RGBPixel> reworkedRGBEven = reworkRGB(pixelsYCbCrEven, H, W);
    std::vector<RGBPixel> reworkedRGBMid = reworkRGB(pixelsYCbCrMid, H, W);

    std::vector<unsigned char> srcR(H * W, 0);
    std::vector<unsigned char> srcG(H * W, 0);
    std::vector<unsigned char> srcB(H * W, 0);
    std::vector<unsigned char> srcY(H * W, 0);
    std::vector<unsigned char> srcCb(H * W, 0);
    std::vector<unsigned char> srcCr(H * W, 0);
    std::vector<unsigned char> reworkedREven(H * W, 0);
    std::vector<unsigned char> reworkedGEven(H * W, 0);
    std::vector<unsigned char> reworkedBEven(H * W, 0);
    std::vector<unsigned char> reworkedRMid(H * W, 0);
    std::vector<unsigned char> reworkedGMid(H * W, 0);
    std::vector<unsigned char> reworkedBMid(H * W, 0);
    std::vector<unsigned char> CbEven(H * W, 0);
    std::vector<unsigned char> CbMid(H * W, 0);
    std::vector<unsigned char> CrEven(H * W, 0);
    std::vector<unsigned char> CrMid(H * W, 0);
    for (int i = 0; i < H * W; i++){
        srcR[i] = pixels[i].R;
        srcG[i] = pixels[i].G;
        srcB[i] = pixels[i].B;
        srcY[i] = YCbCrPixels[i].Y;
        srcCb[i] = YCbCrPixels[i].Cb;
        srcCr[i] = YCbCrPixels[i].Cr;
        reworkedREven[i] = reworkedRGBEven[i].R;
        reworkedGEven[i] = reworkedRGBEven[i].G;
        reworkedBEven[i] = reworkedRGBEven[i].B;
        reworkedRMid[i] = reworkedRGBMid[i].R;
        reworkedGMid[i] = reworkedRGBMid[i].G;
        reworkedBMid[i] = reworkedRGBMid[i].B;
        CbEven[i] = pixelsYCbCrEven[i].Cb;
        CbMid[i] = pixelsYCbCrMid[i].Cb;
        CrEven[i] = pixelsYCbCrEven[i].Cr;
        CbEven[i] = pixelsYCbCrEven[i].Cb;
    }

    pixelsYCbCrEven.clear();
    pixelsYCbCrMid.clear();
    reworkedRGBEven.clear();
    reworkedRGBMid.clear();

    std::cout << "Decimation /4:" << std::endl;
    std::cout << "(Even) srcCb - Cb; PSNR = " << PSNR(srcCb, CbEven) << std::endl;
    std::cout << "(Mid) srcCb - Cb; PSNR = " << PSNR(srcCb, CbMid) << std::endl;
    std::cout << "(Even) srcCr - Cr; PSNR = " << PSNR(srcCb, CrEven) << std::endl;
    std::cout << "(Mid) srcCr - Cr; PSNR = " << PSNR(srcCb, CrMid) << std::endl;
    std::cout << "(Even) srcR - R; PSNR = " << PSNR(srcR, reworkedREven) << std::endl;
    std::cout << "(Even) srcG - G; PSNR = " << PSNR(srcG, reworkedGEven) << std::endl;
    std::cout << "(Even) srcB - B; PSNR = " << PSNR(srcB, reworkedBEven) << std::endl;
    std::cout << "(Mid) srcR - R; PSNR = " << PSNR(srcR, reworkedRMid) << std::endl;
    std::cout << "(Mid) srcG - G; PSNR = " << PSNR(srcG, reworkedGMid) << std::endl;
    std::cout << "(Mid) srcB - B; PSNR = " << PSNR(srcB, reworkedBMid) << std::endl;

    srcR.clear();
    srcB.clear();
    srcG.clear();
    srcY.clear();
    srcCb.clear();
    srcCr.clear();
    reworkedREven.clear();
    reworkedGEven.clear();
    reworkedBEven.clear();
    reworkedRMid.clear();
    reworkedGMid.clear();
    reworkedBMid.clear();
    CbEven.clear();
    CbMid.clear();
    CrEven.clear();
    CrMid.clear();
}

void decimation2(const std::vector<YCbCrPixel>& YCbCrPixels, const std::vector<RGBPixel>& pixels, int H, int W){
    std::vector<YCbCrPixel> pixelsYCbCrEven(H * W, YCbCrPixel(0, 0, 0));
    for (int i = H - 1; i >= 0; i--){
        for (int j = W - 1; j >= 0; j--){
            // Берем всю нижнюю строчку
            if (i % 2 == 0){
                pixelsYCbCrEven[i * W + j] = YCbCrPixels[(i + 1) * W + j];
                pixelsYCbCrEven[i].Y = YCbCrPixels[i].Y;
                continue;
            }
            // Берем слева
            if (j % 2 == 0){
                pixelsYCbCrEven[i * W + j] = YCbCrPixels[i * W + j + 1];
                pixelsYCbCrEven[i].Y = YCbCrPixels[i].Y;
                continue;
            }
            pixelsYCbCrEven[i * W + j] = YCbCrPixels[i * W + j];
            pixelsYCbCrEven[i].Y = YCbCrPixels[i].Y;
        }
    }

    std::vector<YCbCrPixel> pixelsYCbCrMid(H * W, YCbCrPixel(0, 0, 0));
    for (int i = H - 1; i >= 0; i = i - 2){
        for (int j = W - 1; j >= 0; j = j - 2){
            try{
                int tmpCb = YCbCrPixels[i * W + j].Cb + YCbCrPixels[i * W + j - 1].Cb + YCbCrPixels[(i - 1) * W + j].Cb + YCbCrPixels[(i - 1) * W + j - 1].Cb;
                tmpCb /= 4;
                int tmpCr = YCbCrPixels[i * W + j].Cr + YCbCrPixels[i * W + j - 1].Cr + YCbCrPixels[(i - 1) * W + j].Cr + YCbCrPixels[(i - 1) * W + j - 1].Cr;
                tmpCr /= 4;
                YCbCrPixel pixel(YCbCrPixels[i].Y, tmpCb, tmpCr);
                pixelsYCbCrMid[i * W + j] = pixel;
                pixelsYCbCrMid[i * W + j - 1] = pixel;
                pixelsYCbCrMid[(i - 1) * W + j] = pixel;
                pixelsYCbCrMid[(i - 1) * W + j - 1] = pixel;
            }
            catch(const std::out_of_range& e){
                std::cerr << "Out of bounds - std::vector<YCbCrPixel> pixelsYCbCrMid: " << e.what();
                exit(-1);
            }
        }
    }

    std::vector<RGBPixel> reworkedRGBEven = reworkRGB(pixelsYCbCrEven, H, W);
    std::vector<RGBPixel> reworkedRGBMid = reworkRGB(pixelsYCbCrMid, H, W);

    std::vector<unsigned char> srcR(H * W, 0);
    std::vector<unsigned char> srcG(H * W, 0);
    std::vector<unsigned char> srcB(H * W, 0);
    std::vector<unsigned char> srcY(H * W, 0);
    std::vector<unsigned char> srcCb(H * W, 0);
    std::vector<unsigned char> srcCr(H * W, 0);
    std::vector<unsigned char> reworkedREven(H * W, 0);
    std::vector<unsigned char> reworkedGEven(H * W, 0);
    std::vector<unsigned char> reworkedBEven(H * W, 0);
    std::vector<unsigned char> reworkedRMid(H * W, 0);
    std::vector<unsigned char> reworkedGMid(H * W, 0);
    std::vector<unsigned char> reworkedBMid(H * W, 0);
    std::vector<unsigned char> CbEven(H * W, 0);
    std::vector<unsigned char> CbMid(H * W, 0);
    std::vector<unsigned char> CrEven(H * W, 0);
    std::vector<unsigned char> CrMid(H * W, 0);
    for (int i = 0; i < H * W; i++){
        srcR[i] = pixels[i].R;
        srcG[i] = pixels[i].G;
        srcB[i] = pixels[i].B;
        srcY[i] = YCbCrPixels[i].Y;
        srcCb[i] = YCbCrPixels[i].Cb;
        srcCr[i] = YCbCrPixels[i].Cr;
        reworkedREven[i] = reworkedRGBEven[i].R;
        reworkedGEven[i] = reworkedRGBEven[i].G;
        reworkedBEven[i] = reworkedRGBEven[i].B;
        reworkedRMid[i] = reworkedRGBMid[i].R;
        reworkedGMid[i] = reworkedRGBMid[i].G;
        reworkedBMid[i] = reworkedRGBMid[i].B;
        CbEven[i] = pixelsYCbCrEven[i].Cb;
        CbMid[i] = pixelsYCbCrMid[i].Cb;
        CrEven[i] = pixelsYCbCrEven[i].Cr;
        CbEven[i] = pixelsYCbCrEven[i].Cb;
    }

    pixelsYCbCrEven.clear();
    pixelsYCbCrMid.clear();
    reworkedRGBEven.clear();
    reworkedRGBMid.clear();

    std::cout << "Decimation /2:" << std::endl;
    std::cout << "(Even) srcCb - Cb; PSNR = " << PSNR(srcCb, CbEven) << std::endl;
    std::cout << "(Mid) srcCb - Cb; PSNR = " << PSNR(srcCb, CbMid) << std::endl;
    std::cout << "(Even) srcCr - Cr; PSNR = " << PSNR(srcCb, CrEven) << std::endl;
    std::cout << "(Mid) srcCr - Cr; PSNR = " << PSNR(srcCb, CrMid) << std::endl;
    std::cout << "(Even) srcR - R; PSNR = " << PSNR(srcR, reworkedREven) << std::endl;
    std::cout << "(Even) srcG - G; PSNR = " << PSNR(srcG, reworkedGEven) << std::endl;
    std::cout << "(Even) srcB - B; PSNR = " << PSNR(srcB, reworkedBEven) << std::endl;
    std::cout << "(Mid) srcR - R; PSNR = " << PSNR(srcR, reworkedRMid) << std::endl;
    std::cout << "(Mid) srcG - G; PSNR = " << PSNR(srcG, reworkedGMid) << std::endl;
    std::cout << "(Mid) srcB - B; PSNR = " << PSNR(srcB, reworkedBMid) << std::endl;

    srcR.clear();
    srcB.clear();
    srcG.clear();
    srcY.clear();
    srcCb.clear();
    srcCr.clear();
    reworkedREven.clear();
    reworkedGEven.clear();
    reworkedBEven.clear();
    reworkedRMid.clear();
    reworkedGMid.clear();
    reworkedBMid.clear();
    CbEven.clear();
    CbMid.clear();
    CrEven.clear();
    CrMid.clear();
}

#endif //BMP_ANALYSER_DECIMATION_H
