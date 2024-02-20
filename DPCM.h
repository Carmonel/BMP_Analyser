#ifndef BMP_ANALYSER_DPCM_H
#define BMP_ANALYSER_DPCM_H

#include "pixels.h"
#include "histogram.h"
#include "entropy.h"

std::vector<int> DPCMr1(const std::vector<unsigned char>& a, int H, int W, const std::string& outputPath){
    std::vector<int> DPCM(H * W, 0);
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            DPCM[i * W + j] = !((j == 0) || (i == 0)) ? a[i * H + (j - 1)]: a[i * H + j];
        }
    }

    std::vector<int> freq(256, 0);
    for (int i = 0; i < H * W; i++){
        freq[DPCM[i]]++;
    }
    printHistogram(freq, outputPath);

    freq.clear();
    return DPCM;
}

std::vector<int> DPCMr2(const std::vector<unsigned char>& a, int H, int W, const std::string& outputPath){
    std::vector<int> DPCM(H * W, 0);
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            DPCM[i * W + j] = !((j == 0) || (i == 0)) ? a[(i - 1) * H + j]: a[i * H + j];
        }
    }

    std::vector<int> freq(256, 0);
    for (int i = 0; i < H * W; i++){
        freq[DPCM[i]]++;
    }
    printHistogram(freq, outputPath);

    freq.clear();
    return DPCM;
}

std::vector<int> DPCMr3(const std::vector<unsigned char>& a, int H, int W, const std::string& outputPath){
    std::vector<int> DPCM(H * W, 0);
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            DPCM[i * W + j] = !((j == 0) || (i == 0)) ? a[(i - 1) * H + (j - 1)]: a[i * H + j];
        }
    }

    std::vector<int> freq(256, 0);
    for (int i = 0; i < H * W; i++){
        freq[DPCM[i]]++;
    }
    printHistogram(freq, outputPath);

    freq.clear();
    return DPCM;
}

std::vector<int> DPCMr4(const std::vector<unsigned char>& a, int H, int W, const std::string& outputPath){
    std::vector<int> DPCM(H * W, 0);
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            if (((j == 0) || (i == 0))){
                DPCM[i * W + j] = a[i * H + j];
            } else{
                DPCM[i * W + j] = (a[(i - 1) * H + (j - 1)] + a[(i - 1) * H + j] + a[i * H + (j - 1)]) / 3;
            }
        }
    }

    std::vector<int> freq(256, 0);
    for (int i = 0; i < H * W; i++){
        freq[DPCM[i]]++;
    }
    printHistogram(freq, outputPath);

    freq.clear();
    return DPCM;
}

void DPCMforRGB(std::vector<RGBPixel> pixels, int H, int W, const std::string& outputPath){
    std::vector<unsigned char> R(H * W, 0);
    std::vector<unsigned char> G(H * W, 0);
    std::vector<unsigned char> B(H * W, 0);
    for (int i = 0; i < H * W; i++){
        R[i] = pixels[i].R;
        G[i] = pixels[i].G;
        B[i] = pixels[i].B;
    }

    std::vector<int> DPCMr1_R = DPCMr1(R, H, W, outputPath + "DPCMr1_R_freq.txt");
    std::cout << "DPCMr1_R entropy = " << entropy(DPCMr1_R) << std::endl;
    std::vector<int> DPCMr2_R = DPCMr2(R, H, W, outputPath + "DPCMr2_R_freq.txt");
    std::cout << "DPCMr2_R entropy = " << entropy(DPCMr2_R) << std::endl;
    std::vector<int> DPCMr3_R = DPCMr3(R, H, W, outputPath + "DPCMr3_R_freq.txt");
    std::cout << "DPCMr3_R entropy = " << entropy(DPCMr3_R) << std::endl;
    std::vector<int> DPCMr4_R = DPCMr4(R, H, W, outputPath + "DPCMr4_R_freq.txt");
    std::cout << "DPCMr4_R entropy = " << entropy(DPCMr4_R) << std::endl;

    std::vector<int> DPCMr1_G = DPCMr1(G, H, W, outputPath + "DPCMr1_G_freq.txt");
    std::cout << "DPCMr1_G entropy = " << entropy(DPCMr1_G) << std::endl;
    std::vector<int> DPCMr2_G = DPCMr2(G, H, W, outputPath + "DPCMr2_G_freq.txt");
    std::cout << "DPCMr2_G entropy = " << entropy(DPCMr2_G) << std::endl;
    std::vector<int> DPCMr3_G = DPCMr3(G, H, W, outputPath + "DPCMr3_G_freq.txt");
    std::cout << "DPCMr3_G entropy = " << entropy(DPCMr3_G) << std::endl;
    std::vector<int> DPCMr4_G = DPCMr4(G, H, W, outputPath + "DPCMr4_G_freq.txt");
    std::cout << "DPCMr4_G entropy = " << entropy(DPCMr4_G) << std::endl;

    std::vector<int> DPCMr1_B = DPCMr1(B, H, W, outputPath + "DPCMr1_B_freq.txt");
    std::cout << "DPCMr1_B entropy = " << entropy(DPCMr1_B) << std::endl;
    std::vector<int> DPCMr2_B = DPCMr2(B, H, W, outputPath + "DPCMr2_B_freq.txt");
    std::cout << "DPCMr2_B entropy = " << entropy(DPCMr2_B) << std::endl;
    std::vector<int> DPCMr3_B = DPCMr3(B, H, W, outputPath + "DPCMr3_B_freq.txt");
    std::cout << "DPCMr3_B entropy = " << entropy(DPCMr3_B) << std::endl;
    std::vector<int> DPCMr4_B = DPCMr4(B, H, W, outputPath + "DPCMr4_B_freq.txt");
    std::cout << "DPCMr4_B entropy = " << entropy(DPCMr4_B) << std::endl;

    DPCMr1_R.clear();
    DPCMr2_R.clear();
    DPCMr3_R.clear();
    DPCMr4_R.clear();

    DPCMr1_G.clear();
    DPCMr2_G.clear();
    DPCMr3_G.clear();
    DPCMr4_G.clear();

    DPCMr1_B.clear();
    DPCMr2_B.clear();
    DPCMr3_B.clear();
    DPCMr4_B.clear();

    R.clear();
    B.clear();
    G.clear();
}

void DPCMforYCbCr(std::vector<YCbCrPixel> pixels, int H, int W, const std::string& outputPath){
    std::vector<unsigned char> Y(H * W, 0);
    std::vector<unsigned char> Cb(H * W, 0);
    std::vector<unsigned char> Cr(H * W, 0);
    for (int i = 0; i < H * W; i++){
        Y[i] = pixels[i].Y;
        Cb[i] = pixels[i].Cb;
        Cr[i] = pixels[i].Cr;
    }

    std::vector<int> DPCMr1_Y = DPCMr1(Y, H, W, outputPath + "DPCMr1_Y_freq.txt");
    std::cout << "DPCMr1_Y entropy = " << entropy(DPCMr1_Y) << std::endl;
    std::vector<int> DPCMr2_Y = DPCMr2(Y, H, W, outputPath + "DPCMr2_Y_freq.txt");
    std::cout << "DPCMr2_Y entropy = " << entropy(DPCMr2_Y) << std::endl;
    std::vector<int> DPCMr3_Y = DPCMr3(Y, H, W, outputPath + "DPCMr3_Y_freq.txt");
    std::cout << "DPCMr3_Y entropy = " << entropy(DPCMr3_Y) << std::endl;
    std::vector<int> DPCMr4_Y = DPCMr4(Y, H, W, outputPath + "DPCMr4_Y_freq.txt");
    std::cout << "DPCMr4_Y entropy = " << entropy(DPCMr4_Y) << std::endl;

    std::vector<int> DPCMr1_Cb = DPCMr1(Cb, H, W, outputPath + "DPCMr1_Cb_freq.txt");
    std::cout << "DPCMr1_Cb entropy = " << entropy(DPCMr1_Cb) << std::endl;
    std::vector<int> DPCMr2_Cb = DPCMr2(Cb, H, W, outputPath + "DPCMr2_Cb_freq.txt");
    std::cout << "DPCMr2_Cb entropy = " << entropy(DPCMr2_Cb) << std::endl;
    std::vector<int> DPCMr3_Cb = DPCMr3(Cb, H, W, outputPath + "DPCMr3_Cb_freq.txt");
    std::cout << "DPCMr3_Cb entropy = " << entropy(DPCMr3_Cb) << std::endl;
    std::vector<int> DPCMr4_Cb = DPCMr4(Cb, H, W, outputPath + "DPCMr4_Cb_freq.txt");
    std::cout << "DPCMr4_Cb entropy = " << entropy(DPCMr4_Cb) << std::endl;

    std::vector<int> DPCMr1_Cr = DPCMr1(Cr, H, W, outputPath + "DPCMr1_Cr_freq.txt");
    std::cout << "DPCMr1_Cr entropy = " << entropy(DPCMr1_Cr) << std::endl;
    std::vector<int> DPCMr2_Cr = DPCMr2(Cr, H, W, outputPath + "DPCMr2_Cr_freq.txt");
    std::cout << "DPCMr2_Cr entropy = " << entropy(DPCMr2_Cr) << std::endl;
    std::vector<int> DPCMr3_Cr = DPCMr3(Cr, H, W, outputPath + "DPCMr3_Cr_freq.txt");
    std::cout << "DPCMr3_Cr entropy = " << entropy(DPCMr3_Cr) << std::endl;
    std::vector<int> DPCMr4_Cr = DPCMr4(Cr, H, W, outputPath + "DPCMr4_Cr_freq.txt");
    std::cout << "DPCMr4_Cr entropy = " << entropy(DPCMr4_Cr) << std::endl;

    DPCMr1_Y.clear();
    DPCMr2_Y.clear();
    DPCMr3_Y.clear();
    DPCMr4_Y.clear();

    DPCMr1_Cb.clear();
    DPCMr2_Cb.clear();
    DPCMr3_Cb.clear();
    DPCMr4_Cb.clear();

    DPCMr1_Cr.clear();
    DPCMr2_Cr.clear();
    DPCMr3_Cr.clear();
    DPCMr4_Cr.clear();

    Y.clear();
    Cb.clear();
    Cr.clear();
}

#endif //BMP_ANALYSER_DPCM_H
