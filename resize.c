/**
 * Enlarge the picture by specific size.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    // 確保輸入的指令只有4個string (resize function, 放大倍數, 輸入的圖片, 輸出的圖片)
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }
    
    // 確認要放大的圖片倍數是整數
    int resize = atoi(argv[1]);
    if (resize <=0)
    {
        fprintf(stderr, "The size to adjust must be an positive integer\n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // 調整 bi 的header 參數
    bi.biWidth = resize * bi.biWidth;
    bi.biHeight = resize * bi.biHeight;
    
    // determine padding for scanlines
    int old_padding = (4 - (bi.biWidth/resize * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (bi.biWidth* sizeof(RGBTRIPLE)) % 4) % 4;
    
    // 調整 sizeimage / filesize
    bi.biSizeImage = ( sizeof(RGBTRIPLE)*bi.biWidth + new_padding )*abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    
    // iterate over infile's scanlines
    //畫出整張圖
    for (int i = 0, biHeight = abs(bi.biHeight/resize); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        // 在該row 複製每個pixel 及加上padding 後，重複該row 的長度 n 次
        for (int l =0; l<resize; l++)
        {
            // 重複一個row 的寬度
            fseek(inptr, (54 + ((bi.biWidth/resize * 3 + old_padding) * i)), SEEK_SET);
            for (int j = 0; j < bi.biWidth/resize; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
        
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
        
                // write RGB triple to outfile
                // 重複單一pixel 的寬度
                for (int m =0; m<resize; m++)
                {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over padding, if any
            fseek(inptr,old_padding, SEEK_CUR);
            
            // then add it back (to demonstrate how)
            // 畫完每個row 後確認padding 的數量
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
