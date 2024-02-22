Usage: BMP_Analyser.exe -inputFile -outputDirectory

Example: BMP_Analyser.exe E:\image\image.bmp E:\image\

## Attention
Create subdirectories for -outputDirectory:

  - AC\
  - DPCM\
  - Sub\

The results of the corresponding tasks will be stored in these folders.

# TASKS:

1. Coordinate with the instructor on a BMP file in RGB24 format.
2. Load the specified BMP file into memory.
3. Extract the contents of the R, G, and B components and save them in separate files in RGB24 format according to the following rule: bytes not related to the saved component should be zeroed out. This will result in three files with corresponding colors.
4. Analyze the correlation properties of the R, G, and B components using the correlation coefficient estimation formula.
  a) Calculate the correlation coefficient estimates between each pair of image components r_RG, r_GB, and r_RB.
  b) Construct cross-sections of a three-dimensional plot of the normalized autocorrelation function estimate. Here, the autocorrelation function is understood as the correlation coefficient calculated for two samples. The first is formed by the data A[i][j], where i = 1, ..., H - y, and j = 1, ..., W - x. The second sample is formed by A[m][n], where m = y + 1, ..., H, and n = x + 1, ..., W. The values on the y-axis should be fixed at points (0, ±5, ±10). The step along the x-axis should be determined independently. The maximum value of x should not exceed a quarter of the image width W. Thus, it is necessary to construct five cross-sections of the autocorrelation function for each component.
5. Convert the data from RGB to YCbCr format using the formulas. For the Y, Cb, and Cr components, calculate the estimates according to step 4.
6. Save the contents of each of the Y, Cb, and Cr components in a separate file in RGB24 format. The component value for each pixel should be saved in all three bytes corresponding to the R, G, and B positions of that pixel. This will create three files with grayscale intensities.
7. Perform the inverse conversion of data from YCbCr to RGB format using the formulas. For the B, G, and R components, calculate the PSNR value for the original and restored data.
8. Decimate the Cb and Cr components by a factor of two in width and height in the following ways:
  a) excluding even-numbered rows and columns;
  b) calculating the values of the thinned image elements as the arithmetic mean of four adjacent elements from the original image.
9. Restore the original size for the Cb and Cr components by copying the missing values from the left and/or top, then perform the inverse conversion from YCbCr to RGB using the formulas.
10. For the Cb, Cr, B, G, and R components, calculate the PSNR value for the original and restored data.
11. Repeat steps 8-10, reducing the size of the Cb and Cr components by a factor of 4 in width and height.
12. Construct frequency histograms for the R, G, B, Y, Cb, and Cr components. Histograms for each component should be constructed on a separate plot. Take the Cb and Cr components with their original sizes.
13. Estimate the number of bits spent on element-wise independent compression of the R, G, B, Y, Cb, and Cr components using the formula.

14. Quantitative analysis of the efficiency of differential coding: form arrays of differences D_A_(r), where A indicates the component of the original image for which the difference is calculated, and r is the number of the difference calculation rule. The value at position (i, j) should be calculated according to the following formula:
The following prediction rules should be used in the study:
  1 - left neighbor (at position (i, j - 1));
  2 - top neighbor (at position (i - 1, 1));
  3 - top left neighbor (at position (i - 1, j - 1));
  4 - arithmetic mean of three neighbors above, left, and above left.
Arrays should be constructed for the R, G, B, Y, Cb, and Cr components.
15. For the D_A_(r) arrays formed in the previous task, construct frequency histograms and compare them with the histograms of the values of the corresponding components.
16. Estimate the number of bits spent on element-wise independent compression of the D_A_(r) arrays using the formula. Compare the obtained values with the entropy estimates for the corresponding components.
17. Represent the original luma component Y as four sub-frames Y(i,j) (i, j = 0,1), which are formed as follows: Y(i,j)(y, x) = Y(2y + i, 2x + j). For example, frame Y(0,0) is formed from the luma values of pixels located in even rows and even columns of the original image. Calculate the correlation coefficient between each pair of sub-frames.
