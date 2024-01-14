/**
 * @file fn-imgio-gpl.cpp
 * @author Junichi Nishikata
 */

#include "fn-tiff_io.h"

#include <wsp/common/_tpl_fn-array.hpp>

#include <tiff.h>
#include <tiffio.h>

#include <iostream>
#include <stdlib.h>

using namespace std;

namespace{
    wsp::State _MergeChannels(uchar *o_dst, u32 dst_spp, const uchar *in_data, u32 src_spp, const uchar *in_alpha, u32 width, u32 height)
    {
        u32 imgIndex, yIndex, dst_index, src_index;
        u32 dataLen = width * height;
        uchar *tmp_alpha;

        if (dst_spp == src_spp)
        {
            WSP_COMMON_ERROR_LOG("spp of source and destination is the same\n");
            return WSP_STATE_INVALID_PARAMETER;
        }

        if (0 >= dst_spp || dst_spp < src_spp)
        {
            WSP_COMMON_ERROR_LOG("Samples Per Pixel are wrong\n");
            return WSP_STATE_FAILURE;
        }

        u32 i, c;
        u32 len = width*height;
        for (i = 0; i < len; i++)
        {
            src_index = i*src_spp;
            dst_index = i*dst_spp;
            for (c = 0; c < dst_spp - 1; c++)
            {
                if (i >= src_spp)
                {
                    o_dst[dst_index + c] = in_data[src_index + src_spp - 1];
                    continue;
                }

                o_dst[dst_index + c] = in_data[src_index + c];
            }
            if (in_alpha == NULL)
            {
                continue;
            }

            o_dst[dst_index + dst_spp - 1] = in_alpha[i];
        }

        return WSP_STATE_SUCCESS;
    }
}

//! Tiff I/O ------------------------------------------------------------------------------
void wsp::libtiff::LoadTiff32(u8 *O_image, u32 &o_width, u32 &o_height, const char *filename)
{
    tsize_t stripSize;
    TIFF *image;
    uchar *raster8, tempbyte;
    u32 photo, bps, spp, fillorder;
    u32 itr_x, itr_y, itr_i; //! iterators
    u32 width, height;
    u32 index, indexY, dstIndex, srcIndex;
    u32 imageOffset, result;
    u32 rasterSize, numOfStrip;

    //! open the TIFF image
    if ((image = TIFFOpen(filename, "r")) == NULL)
    {
        WSP_COMMON_ERROR_LOG("Could not open incoming image\n");
        return;
    }

    //! get resolusion
    if (TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width) == 0)
    {
        WSP_COMMON_ERROR_LOG("Image does not define its width\n"); TIFFClose(image);
        TIFFClose(image);
        return;
    }
    if (TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height) == 0)
    {
        WSP_COMMON_ERROR_LOG("Image does not define its height\n"); TIFFClose(image);
        TIFFClose(image);
        return;
    }

    //! check image type
    if (TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &bps) == 0)
    {
        WSP_COMMON_ERROR_LOG("Either undefined or unsupported number of bits per sample\n");
        TIFFClose(image); 
        return;
    }
    if (TIFFGetField(image, TIFFTAG_SAMPLESPERPIXEL, &spp) == 0)
    {
        WSP_COMMON_ERROR_LOG("Either undefined or unsupported number of samples per pixel\n");
        TIFFClose(image);
        return;
    }

    //! Read in the possibly multiple strips
    stripSize = TIFFStripSize(image);
    numOfStrip = TIFFNumberOfStrips(image);
    imageOffset = 0;

    rasterSize = numOfStrip * stripSize;

    cout << "bps = " << bps << ", spp = " << spp << ", ";
    cout << "stripSize = " << stripSize << ", numOfStrip = " << numOfStrip << ", rasterSize = " << rasterSize << endl;

    raster8 = new uchar[rasterSize];
    if (raster8 == NULL)
    {
        WSP_COMMON_ERROR_LOG("Could not allocate enough memory\n"); 
        TIFFClose(image); 
        return ;
    }

    for (itr_i = 0; itr_i < numOfStrip; itr_i++)
    {
        if ((result = TIFFReadEncodedStrip(image, itr_i, raster8 + imageOffset, stripSize)) == -1)
        {
            WSP_COMMON_ERROR_LOG("Read error on input strip number %d\n", itr_i); 
            delete[] raster8; TIFFClose(image); 
            return;
        }
        //cout<<result<<" ";  
        imageOffset += result;
    }

    //! deal with photometric interpretation
    if (TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &photo) == 0)
    {
        WSP_COMMON_ERROR_LOG("image has an undefined photometric interpretation\n");
    }

    if (photo == PHOTOMETRIC_MINISWHITE)
    {
        printf("Fixing the photometric interpretation\n");
        for (itr_i = 0; itr_i < rasterSize; itr_i++)
        {
            raster8[itr_i] = ~raster8[itr_i];
        }
    }

    //! deal with fillorder
    if (TIFFGetField(image, TIFFTAG_FILLORDER, &fillorder) == 0)
    {
        WSP_COMMON_ERROR_LOG("Image has an undefined fillorder\n");
    }

    if (fillorder == FILLORDER_LSB2MSB)
    {
        //! We need to swap bits -- ABCDEFGH becomes HGFEDCBA
        printf("Fixing the fillorder\n");
        for (itr_i = 0; itr_i < rasterSize; itr_i++)
        {
            tempbyte = 0;
            if (raster8[itr_i] & 128) tempbyte += 1;
            if (raster8[itr_i] & 64) tempbyte += 2;
            if (raster8[itr_i] & 32) tempbyte += 4;
            if (raster8[itr_i] & 16) tempbyte += 8;
            if (raster8[itr_i] & 8) tempbyte += 16;
            if (raster8[itr_i] & 4) tempbyte += 32;
            if (raster8[itr_i] & 2) tempbyte += 64;
            if (raster8[itr_i] & 1) tempbyte += 128;
            raster8[itr_i] = tempbyte;
        }
    }

    O_image = new uchar[width*height * 4];
    if (O_image == NULL)
    {
        WSP_COMMON_ERROR_LOG("Could not allocate enough memory\n");
        TIFFClose(image); delete[] raster8;
        return;
    }

    for (itr_y = 0; itr_y < height; itr_y++){
        indexY = itr_y*width;
        for (itr_x = 0; itr_x < width; itr_x++)
        {
            index = indexY + itr_x;
            srcIndex = index * spp;
            dstIndex = index * 4;
            //! RGB channels
            for (itr_i = 0; itr_i<3; itr_i++){
                if (itr_i >= spp){
                    O_image[dstIndex + itr_i] = raster8[srcIndex + spp - 1];
                }
                O_image[dstIndex + itr_i] = raster8[srcIndex + itr_i];
            }
            //! Alpha channel
            if (spp>3){
                O_image[dstIndex + 3] = raster8[srcIndex + 3];
                continue;
            }
            O_image[dstIndex + 3] = 255; //! no transparency
        }
    }

    cout << filename << " is successfully loaded!" << endl;

    //! freeing memroy
    TIFFClose(image);
    delete[] raster8;

    o_width = width;
    o_height = height;
}

wsp::State wsp::libtiff::SaveAsTiff32(const uchar *in_img, u32 width, u32 height, const char *filename)
{
    tsize_t numData;
    tdata_t raster8;
    u32 bps, spp;
    TIFF *image;

    if (in_img == NULL){ WSP_COMMON_ERROR_LOG("input image is NULL\n"); return WSP_STATE_FAILURE; }

    bps = 8;
    spp = 4;
    numData = width*height*spp;

    cout << "Saving " << filename << endl;

    raster8 = new uchar[numData];
    if (raster8 == NULL){ WSP_COMMON_ERROR_LOG("memory allocation failed\n"); return WSP_STATE_FAILURE; }

    image = TIFFOpen(filename, "w");
    if (image == NULL)
    {
        WSP_COMMON_ERROR_LOG("Could not open %s\n", filename); delete[] raster8; return WSP_STATE_FAILURE;
    }

    wsp::CopyArray((uchar*)raster8, in_img, numData);

    //! Output Parameter Settings
    TIFFSetField(image, TIFFTAG_IMAGEWIDTH, width); //! width
    TIFFSetField(image, TIFFTAG_IMAGELENGTH, height); //! height
    TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, bps); //! bits/channel
    TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, spp); //! number of channel
    TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, height);
    TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_NONE); //! NONE, CCITTRLE, CCITTFAX3, CCITTFAX4, LZW, OJEPEG, JPEG, DEFLATE
    TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB); //! MINIWHITE, MINIBLACK, RGB
    TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB); //! MSB2LSB (1)-Default-save from upper bit to lower bit, LSB2MSB (2)-save from lower bit to upper bit
    TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(image, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT); //! place of origin
    TIFFSetField(image, TIFFTAG_XRESOLUTION, 72.0); //! dpi x
    TIFFSetField(image, TIFFTAG_YRESOLUTION, 72.0); //! dpi y
    TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_CENTIMETER); //! NONE, INCH, CENTIMETER
    TIFFSetField(image, TIFFTAG_SOFTWARE, "WspLib"); //! name of output software

    //! write tiff file
    TIFFWriteEncodedStrip(image, 0, raster8, numData);

    //! freeing memroy
    TIFFClose(image);
    delete[] raster8;
    return WSP_STATE_SUCCESS;
}

wsp::State wsp::libtiff::SaveAsTiff8(const uchar *in_img, const uchar *in_alphaData, u32 width, u32 height, const char *filename){
    u32 dataLen = height*width;
    wsp::State st;
    uchar *tmp_imgRgba = new uchar[dataLen * 4];
    _MergeChannels(tmp_imgRgba, 4, in_img, 1, in_alphaData, width, height);    //! Convert RGB to RGBA
    st = wsp::libtiff::SaveAsTiff32(tmp_imgRgba, width, height, filename);
    if (st == WSP_STATE_FAILURE){ delete[] tmp_imgRgba; return WSP_STATE_FAILURE; }
    delete[] tmp_imgRgba;
    return WSP_STATE_SUCCESS;
}

wsp::State wsp::libtiff::SaveAsTiff24(const uchar *in_img, const uchar *in_alphaData, u32 width, u32 height, const char *filename){
    u32 dataLen = height*width;
    wsp::State st;
    uchar *tmp_imgRgba = new uchar[dataLen * 4];
    _MergeChannels(tmp_imgRgba, 4, in_img, 3, in_alphaData, width, height);    //! Convert RGB to RGBA
    st = wsp::libtiff::SaveAsTiff32(tmp_imgRgba, width, height, filename);
    if (st == WSP_STATE_FAILURE){ delete[] tmp_imgRgba; return WSP_STATE_FAILURE; }
    delete[] tmp_imgRgba;
    return WSP_STATE_SUCCESS;
}

