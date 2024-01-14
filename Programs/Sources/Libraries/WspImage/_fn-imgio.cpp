/**
 * @file fn-imageio.cpp
 * 
 */

#include <iostream>
#include <stdlib.h>

#include <wsp/common/_tpl_fn-mem_manag.hpp>
#include <wsp/common/_tpl_fn-array.hpp>
#include <wsp/common/fn-debug.h>
#include <wsp/common/ScopedFileOpen.h>

#include "_fn-imgio.h"

using namespace std;

namespace{
    // swapping byte of intensity in the image.
    // Example, 0231 -> 1320
    void SwapBytes(u8 *io_img, u32 spp, u32 len)
    {
        WSP_ASSERT_NOT_NULL(io_img);
        const int g_one = 1;
        const bool needs_swap = (((char *)(&g_one))[0] == 0) ? false : true;
        if (needs_swap == false)
        {
            return;
        }

        uchar *ptr, c;
        u32 i, k;

        ptr = io_img;
        for (i = 0; i < len; i++){
            for (k = 0; k < spp / 2; k++)
            {
                c = ptr[k];
                ptr[k] = ptr[spp - k - 1];
                ptr[spp - k - 1] = c;
            }
            ptr += spp;
        }
    }

    /* Bitmapファイルヘッダ */
    typedef struct _tmpBMPFileHeader {
        unsigned short bfType;            /* ファイルタイプ */
        unsigned int bfSize;            /* ファイルサイズ */
        unsigned short bfReserved1;        /* 予約領域 */
        unsigned short bfReserved2;        /* 予約領域 */
        unsigned int bfOffBits;            /* ファイル先頭から画像データまでのオフセット */
    } BMPFileHeader;

    /* BMP情報ヘッダ */
    typedef struct _tmpBMPInfoHeader {
        unsigned int biSize;            /* 情報ヘッダーのサイズ */
        int biWidth;                    /* 幅 */
        int biHeight;                    /* 高さ(正ならば下から上、負ならば上から下) */
        unsigned short biPlanes;        /* プレーン数(常に1) */
        unsigned short biBitCount;        /* 1画素あたりのビット数(1,4,8,24,32) */
        unsigned int biCompression;
        unsigned int biSizeImage;
        int biXPelsPerMeter;
        int biYPelsPerMeter;
        unsigned int biClrUsed;            /* パレットの色数 */
        unsigned int biClrImportant;
    } BMPInfoHeader;

    /* パレットデータ */
    typedef struct _tmpRGBQuad {
        unsigned char rgbBlue;
        unsigned char rgbGreen;
        unsigned char rgbRed;
        unsigned char rgbReserved;
    } RGBQuad;

#define MAX_COLOR_PALETTE        (256)


    /* 画像座標系 */
    /* 1: 左下原点(OpenGL仕様), 0:左上原点 */
#define OPENGL_IMAGE_COORDINATE        (1)

    /* glutBitmapCharacter用フォント */
#ifdef F9X15
#define FUTL_FONT_TYPE        GLUT_BITMAP_9_BY_15
#define FUTL_FONT_HEIGHT    (15)
#else
#define FUTL_FONT_TYPE        GLUT_BITMAP_8_BY_13
#define FUTL_FONT_HEIGHT    (13)
#endif


    /* シフトによる積算 */
#define FUTL_MULT2(x)        ((x) << 1)
#define FUTL_MULT4(x)         ((x) << 2)
#define FUTL_MULT8(x)         ((x) << 3)
#define FUTL_MULT16(x)        ((x) << 4)
#define FUTL_MULT32(x)        ((x) << 5)
#define FUTL_MULT64(x)        ((x) << 6)
#define FUTL_MULT128(x)        ((x) << 7)
#define FUTL_MULT256(x)        ((x) << 8)
#define FUTL_NMULT(x, n)    ((x) << (n))

    /* シフトによる除算 */
#define FUTL_DIV2(x)        ((x) >> 1)
#define FUTL_DIV4(x)         ((x) >> 2)
#define FUTL_DIV8(x)         ((x) >> 3)
#define FUTL_DIV16(x)        ((x) >> 4)
#define FUTL_DIV32(x)        ((x) >> 5)
#define FUTL_DIV64(x)        ((x) >> 6)
#define FUTL_DIV128(x)        ((x) >> 7)
#define FUTL_DIV256(x)        ((x) >> 8)
#define FUTL_DIV512(x)        ((x) >> 9)
#define FUTL_DIV1024(x)        ((x) >> 10)
#define FUTL_DIV2048(x)        ((x) >> 11)
#define FUTL_NDIV(x, n)        ((x) >> (n))

    /* ビット剰余算 */
#define FUTL_MOD2(x)        ((x) & 0x1)
#define FUTL_MOD4(x)         ((x) & 0x3)
#define FUTL_MOD8(x)         ((x) & 0x7)
#define FUTL_MOD16(x)        ((x) & 0xF)
#define FUTL_MOD32(x)        ((x) & 0x1F)
#define FUTL_MOD64(x)        ((x) & 0x3F)
#define FUTL_MOD128(x)        ((x) & 0x7F)
#define FUTL_MOD256(x)        ((x) & 0xFF)
#define FUTL_MOD512(x)        ((x) & 0x1FF)
#define FUTL_MOD1024(x)        ((x) & 0x3FF)
#define FUTL_NMOD(x, n)        ((x) & ((n) - 1))

#define FUTL_ALLIGN4(x)        ((x) & (~0x3))
#define FUTL_ALLIGN32(x)    ((x) & (~0x1F))

#define FUTL_UPALLIGN4(x)    FUTL_ALLIGN4((x) + 0x3)
#define FUTL_UPALLIGN32(x)    FUTL_ALLIGN32((x) + 0x1F)
}

//! Portable Any Map I/O -----------------------------------------------------------------------

wsp::State wsp::img::LoadPNM(u8 **o_rgb24, int *o_width, int *o_height, const char *filename)
{
    unsigned char line[70], c;
    int i, type_read = 0, num_read = 0, num_max = 3;
    int num[3];
    int xsize, ysize, max;
    int is_text = 0;
    int data_size;
    wsp::img::ImageFormat type;

    cout << "LoadPortableImage: "<< filename << endl;

    wsp::common::ScopedOpenFile scoped_open_file(filename, "rb");
    FILE *fp = scoped_open_file.GetOpenedFilePointer();
    if (fp==NULL){
        WSP_IMAGE_ERROR_LOG("Error in LoadPortableImage: %s dosn't exist\n", filename);
        return WSP_STATE_FAILURE;
    }

    while (fgets((char *)line, sizeof(line), fp))
    {
        i = 0;

        if (!type_read)
        {
            if (line[0] == 'P')
            {
                switch (line[1])
                {
                    case '5': type=WSP_IMAGE_FORMAT_PGM; break;
                    case '6': type=WSP_IMAGE_FORMAT_PPM; break;
                    default:  return WSP_STATE_FAILURE;
                }
            }
            else {  return WSP_STATE_FAILURE; }
            if (isdigit(line[2])) {  return WSP_STATE_FAILURE; }
            i = 2;
            type_read = 1;
        }

        for (; c=line[i]; i++)
        {
            if (c == '#') break;
            if (isdigit(c))
            {
                if (num_read >= num_max) {  return WSP_STATE_FAILURE; }
                num[num_read] = c - '0';
                for (; c=line[i+1]; i++)
                {
                    if (!isdigit(c)) break;
                    num[num_read] = 10*num[num_read] + c - '0';
                }
                num_read ++;
            }
        }

        if (num_read >= num_max) break;
    }

    if (num_read != num_max) {  return WSP_STATE_FAILURE; }

    xsize = num[0];
    ysize = num[1];
    max = num[2];

    *o_width = xsize;
    *o_height = ysize;

    wsp::ReallocArrayMem(o_rgb24, xsize*ysize*3);

    switch(type){
        case WSP_IMAGE_FORMAT_PGM:
            data_size = sizeof(uchar);
            break;
        case WSP_IMAGE_FORMAT_PPM:
            data_size = sizeof(uchar[3]);
            break;
        default:
            
            return WSP_STATE_FAILURE;
    }

    if (is_text)
    {
        num_read = 0;
        if (type == WSP_IMAGE_FORMAT_PPM){
            num_max = 3*xsize*ysize;
        }else{
            num_max = xsize*ysize;
        }

        while (fgets((char *)line, sizeof(line), fp))
        {
            for (i=0; c=line[i]; i++)
            {
                if (c == '#') break;
                if (isdigit(c))
                {
                    int tmp;
                    if (num_read >= num_max) {  return WSP_STATE_FAILURE; }
                    tmp = c - '0';
                    for (; c=line[i+1]; i++)
                    {
                        if (!isdigit(c)) break;
                        tmp = 10*tmp + c - '0';
                    }
                    if (type==WSP_IMAGE_FORMAT_PGM){
                        (*o_rgb24)[num_read] = tmp;
                        (*o_rgb24)[num_read+1] = tmp;
                        (*o_rgb24)[num_read+2] = tmp;
                    }
                    else
                    {
                        switch (num_read % 3)
                        {
                            case 0: (*o_rgb24)[num_read] = tmp; break;
                            case 1: (*o_rgb24)[num_read+1] = tmp; break;
                            case 2: (*o_rgb24)[num_read+2] = tmp; break;
                        }
                    }
                    num_read ++;
                }
            }
        }

        if (num_read != num_max) {  return WSP_STATE_FAILURE; }
    }
    else
    {
        size_t size = fread(*o_rgb24, data_size, (size_t)(xsize*ysize), fp);
        if (size != (size_t)(xsize*ysize))
        { WSP_IMAGE_ERROR_LOG("fread failed, %zu!=%d\n", size, xsize*ysize);  return WSP_STATE_FAILURE; }
    }

    
    cout<<"Portable Image successfully loaded"<<endl;

    return WSP_STATE_SUCCESS;
}

void wsp::img::FreePNM(u8 **io_image)
{
    wsp::FreeArrayMem(io_image);
}

wsp::State wsp::img::SaveU24AsPPM(const u8 *in_rgb24, int width, int height, const char *filename)
{
    FILE *fp;
    int i, xsize, ysize;
    int im_max = 0;
    int data_size = sizeof(uchar[3]);

    cout<<"Saving as PPM.."<<endl;
    
    xsize = (int)width;
    ysize = (int)height;

    fp = fopen(filename, "wb");
    if (fp==NULL){ return WSP_STATE_FAILURE; }

    for (i=0; i<xsize*ysize; i++)
    {
        if (im_max < in_rgb24[i*3]){ im_max = in_rgb24[i*3]; }
        if (im_max < in_rgb24[i*3+1]){ im_max = in_rgb24[i*3+1]; }
        if (im_max < in_rgb24[i*3+2]){ im_max = in_rgb24[i*3+2]; }
    }
    fprintf(fp, "P6\n%d %d\n%d\n", xsize, ysize, im_max);

    i = fwrite(in_rgb24, data_size, xsize*ysize, fp);
    if (i != xsize*ysize) { fclose(fp); return WSP_STATE_FAILURE; }

    fclose(fp);
    return WSP_STATE_SUCCESS;
}

wsp::State wsp::img::SaveU8AsPGM(const u8 *in_u8, int width, int height, const char *filename)
{
    int i, xsize, ysize;
    FILE *fp;
    int im_max = 0;
    int data_size = sizeof(uchar);

    cout<<"Saving as Portable Image.."<<endl;
    
    //wsp::img::ImageFormat type = WSP_IMAGE_FORMAT_PPM;

    xsize = (int)width;
    ysize = (int)height;

    fp = fopen(filename, "wb");
    if (fp==NULL){ return WSP_STATE_FAILURE; }

    for (i=0; i<xsize*ysize; i++){
        if (im_max < in_u8[i]){ im_max = in_u8[i]; }
    }
    fprintf(fp, "P5\n%d %d\n%d\n", xsize, ysize, im_max);

    i = fwrite(in_u8, data_size, xsize*ysize, fp);
    if (i != xsize*ysize) { fclose(fp); return WSP_STATE_FAILURE; }

    fclose(fp);

    return WSP_STATE_SUCCESS;    
}

/*
* BMPファイルのロード
*/
wsp::State wsp::img::LoadBitmapImage(u8 **o_data, int *o_width, int *o_height, int *o_channel_count, const char *file_path)
{
    WSP_REQUIRES_NOT_NULL(o_data);
#ifdef _WIN32
    errno_t err;
#endif
    FILE *fp;
    int i, j;
    int bpp, width, height;
    int lineBits, lineSize, linePos;
    unsigned char *lpImg, *lpImgArea;
    int index;
    unsigned char bfType[2];
    unsigned int bfOffBits;
    BMPInfoHeader bmpInfo;
    RGBQuad palette[MAX_COLOR_PALETTE];

#ifdef _WIN32
    if ((err = fopen_s(&fp, file_path, "rb")) != 0)
#else
    if ((fp = fopen(name, "rb")) == NULL)
#endif
    {
        return WSP_STATE_NOT_FOUND;
    }

    /* ファイルサイズ */
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) < (sizeof(BMPFileHeader) + sizeof(BMPInfoHeader)))
    {
        fclose(fp);
        return WSP_STATE_UNSUPPORTED_TYPE;
    }

    /* ファイルタイプ */
    fseek(fp, 0, SEEK_SET);
    fread(&bfType[0], 1, 2, fp);
    if (bfType[0] != 'B' || bfType[1] != 'M')
    {
        fclose(fp);
        return WSP_STATE_UNSUPPORTED_TYPE;
    }

    /* 画像データへのオフセット */
    fseek(fp, 10, SEEK_SET);
    fread(&bfOffBits, 4, 1, fp);

    /* BMP情報ヘッダ読み込み */
    fseek(fp, 14, SEEK_SET);
    fread(&bmpInfo, sizeof(BMPInfoHeader), 1, fp);
    if (bmpInfo.biCompression != 0 || bmpInfo.biSize != 40)
    {
        /* ランレングス圧縮は未サポート */
        fclose(fp);
        return WSP_STATE_UNSUPPORTED_TYPE;
    }

    /* 1画素当たりのビット数、幅、高さ */
    bpp = bmpInfo.biBitCount;
    width = bmpInfo.biWidth;
    height = bmpInfo.biHeight;

    /* パッドも含めたラインサイズ（バイト） */
    lineBits = FUTL_UPALLIGN32(bpp * width);
    lineSize = lineBits / 8;

    /* RGBA領域確保 */
    lpImgArea = (unsigned char *)malloc(sizeof(unsigned int) * width * height);
    if (lpImgArea == NULL)
    {
        fclose(fp);
        return WSP_STATE_INSUFFICIENT_MEMORY;
    }

    /* RGB読み込み */
    lpImg = lpImgArea;
    switch (bpp)
    {
    case 8:    /* 8 bits/pixel */
               /* カラーマップ読み込み */
        fseek(fp, 54, SEEK_SET);
        fread(&palette[0], sizeof(RGBQuad), 256, fp);

#if OPENGL_IMAGE_COORDINATE != 0
        for (i = 0; i < height; i++)
#else
        for (i = height - 1; i >= 0; i--)
#endif
        {
            linePos = bfOffBits + lineSize * i;
            fseek(fp, linePos, SEEK_SET);
            for (j = 0; j < width; j++)
            {
                index = fgetc(fp);
                *lpImg++ = palette[index].rgbRed;
                *lpImg++ = palette[index].rgbGreen;
                *lpImg++ = palette[index].rgbBlue;
                *lpImg++ = 255;
            }
        }
        break;

    case 24: /* 24 bits/pixel */
#if OPENGL_IMAGE_COORDINATE != 0
        for (i = 0; i < height; i++)
#else
        for (i = height - 1; i >= 0; i--)
#endif
        {
            linePos = bfOffBits + lineSize * i;
            fseek(fp, linePos, SEEK_SET);
            for (j = 0; j < width; j++)
            {
                *(lpImg + 2) = (unsigned char)fgetc(fp);    /* B */
                *(lpImg + 1) = (unsigned char)fgetc(fp);    /* G */
                *(lpImg + 0) = (unsigned char)fgetc(fp);    /* R */
                *(lpImg + 3) = 255;                            /* A */
                lpImg += 4;
            }
        }
        break;

    case 32: /* 32 bits/pixel */
#if OPENGL_IMAGE_COORDINATE != 0
        for (i = 0; i < height; i++)
#else
        for (i = height - 1; i >= 0; i--)
#endif
        {
            linePos = bfOffBits + lineSize * i;
            fseek(fp, linePos, SEEK_SET);
            for (j = 0; j < width; j++)
            {
                unsigned char dummy;
                *(lpImg + 2) = (unsigned char)fgetc(fp);    /* B */
                *(lpImg + 1) = (unsigned char)fgetc(fp);    /* G */
                *(lpImg + 0) = (unsigned char)fgetc(fp);    /* R */
                dummy = (unsigned char)fgetc(fp);    /* reserved */
                *(lpImg + 3) = 255;                            /* A */
                lpImg += 4;
            }
        }
        break;

    default:
        free(lpImg);
        fclose(fp);
        return WSP_STATE_UNSUPPORTED_TYPE;
    }

    *o_data = lpImgArea;
    *o_width = width;
    *o_height = height;
    *o_channel_count = 4;

    fclose(fp);

    return WSP_STATE_SUCCESS;
}


void wsp::img::FreeBitmapImage(u8 **io_data)
{
    if (*io_data == NULL)
    {
        return;
    }
    else if (*io_data != NULL)
    {
        free(*io_data);
        *io_data = NULL;
    }
}