/**
 * @file fn-imgio.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <wsp/common/fn-io.h>
#include "fn-imgio.h"

//! Portable Any Map I/O -----------------------------------------------------------------------

WSP_ImageState WSP_LoadPNM(u8 *o_rgb24, int *o_width, int *o_height,  const char *filename)
{
    FILE *fp;
    unsigned char line[70], c;
    int i, type_read = 0, num_read = 0, num_max = 3;
    int num[3];
    int xsize, ysize, max;
    int is_text = 0;
    int data_size, size;
    WSP_ImageFormat type;

    WSP_COMMON_DEBUG_LOG( "filename = %s\n", filename);

    fp = fopen(filename, "rb");
    if (fp==NULL){
        WSP_COMMON_ERROR_LOG("%s dosn't exist\n", filename);
        return WSP_IMAGE_STATE_COULDNT_OPEN;
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
                default: 
                    fclose(fp); 
                    return WSP_IMAGE_STATE_UNSUPPORTED_FORMAT;
                }
            }
            else 
            { 
                fclose(fp); 
                return WSP_IMAGE_STATE_UNSUPPORTED_FORMAT; 
            }

            if (isdigit(line[2])) 
            { 
                fclose(fp); 
                return WSP_IMAGE_STATE_INVALID_FORMAT; 
            }
            i = 2;
            type_read = 1;
        }

        for (; c=line[i]; i++)
        {
            if (c == '#') break;
            if (isdigit(c))
            {
                if (num_read >= num_max) 
                { 
                    fclose(fp); 
                    return WSP_IMAGE_STATE_INVALID_FORMAT; 
                }
                num[num_read] = c - '0';
                for (; c=line[i+1]; i++)
                {
                    if (!isdigit(c)) break;
                    num[num_read] = 10*num[num_read] + c - '0';
                }
                num_read ++;
            }
        }

        if (num_read >= num_max){ break;}
    }

    if (num_read != num_max) { fclose(fp); return WSP_IMAGE_STATE_INVALID_FORMAT; }

    xsize = num[0];
    ysize = num[1];
    max = num[2];

    *o_width = xsize;
    *o_height = ysize;

    //o_rgb24 = new u8[xsize*ysize*3];
    o_rgb24 = (u8*)malloc( xsize * ysize * 3 * sizeof(u8) );

    switch(type){
    case WSP_IMAGE_FORMAT_PGM:
        data_size = sizeof(u8);
        break;
    case WSP_IMAGE_FORMAT_PPM:
        data_size = sizeof(u8[3]);
        break;
    default:
        return WSP_IMAGE_STATE_UNSUPPORTED_FORMAT;
    }

    if (is_text)
    {
        num_read = 0;
        switch(type){
        case WSP_IMAGE_FORMAT_PGM:
            num_max = 3*xsize*ysize;
            break;
        case WSP_IMAGE_FORMAT_PPM:
            num_max = xsize*ysize;
            break;
        default:
            return WSP_IMAGE_STATE_UNSUPPORTED_FORMAT;
        }

        while (fgets((char *)line, sizeof(line), fp))
        {
            for (i=0; c=line[i]; i++)
            {
                if (c == '#'){ break; }

                if (isdigit(c))
                {
                    int tmp;
                    if (num_read >= num_max) 
                    { 
                        fclose(fp); 
                        return WSP_IMAGE_STATE_INVALID_FORMAT; 
                    }
                    tmp = c - '0';
                    for (; c=line[i+1]; i++)
                    {
                        if (!isdigit(c)) break;
                        tmp = 10*tmp + c - '0';
                    }
                    if (type==WSP_IMAGE_FORMAT_PGM){
                        o_rgb24[num_read] = tmp;
                        o_rgb24[num_read+1] = tmp;
                        o_rgb24[num_read+2] = tmp;
                    }
                    else
                    {
                        switch (num_read % 3)
                        {
                        case 0: o_rgb24[num_read] = tmp; break;
                        case 1: o_rgb24[num_read+1] = tmp; break;
                        case 2: o_rgb24[num_read+2] = tmp; break;
                        }
                    }
                    ++num_read;
                }
            }
        }

        if (num_read != num_max) { fclose(fp); return WSP_IMAGE_STATE_INVALID_FORMAT; }
    }
    else
    {
        size = fread(o_rgb24, data_size, (size_t)(xsize*ysize), fp);
        if (size != (size_t)(xsize*ysize))
        { 
            WSP_COMMON_ERROR_LOG("fread failed, %d!=%d\n", size, xsize*ysize); 
            fclose(fp); 
            return WSP_IMAGE_STATE_READING_FILE_FAILED; 
        }
    }

    fclose(fp);
    WSP_COMMON_DEBUG_LOG("Portable Image successfully loaded\n");

    return WSP_IMAGE_STATE_SUCCESS;
}

WSP_ImageState WSP_SaveU24AsPPM(const u8 *in_rgb24, int width, int height, const char *filename)
{
    FILE *fp;
    int i, xsize, ysize;
    int im_max = 0;
    int data_size = sizeof(u8[3]);

    WSP_COMMON_DEBUG_LOG("Saving as PPM..\n");
    
    xsize = (int)width;
    ysize = (int)height;

    fp = fopen(filename, "wb");
    if (fp==NULL){ return WSP_IMAGE_STATE_COULDNT_OPEN; }

    for (i=0; i<xsize*ysize; i++)
    {
        if (im_max < in_rgb24[i*3]){ im_max = in_rgb24[i*3]; }
        if (im_max < in_rgb24[i*3+1]){ im_max = in_rgb24[i*3+1]; }
        if (im_max < in_rgb24[i*3+2]){ im_max = in_rgb24[i*3+2]; }
    }
    fprintf(fp, "P6\n%d %d\n%d\n", xsize, ysize, im_max);

    i = fwrite(in_rgb24, data_size, xsize*ysize, fp);
    fclose(fp); 
    if (i != xsize*ysize) 
    { 
        return WSP_IMAGE_STATE_WRITING_FILE_FAILED; 
    }

    return WSP_IMAGE_STATE_SUCCESS;
}

WSP_ImageState WSP_SaveU8AsPGM(const u8 *in_u8, int width, int height, const char *filename)
{
    int i, xsize, ysize;
    FILE *fp;
    int im_max = 0;
    int data_size = sizeof(u8);

    WSP_COMMON_DEBUG_LOG("Saving as Portable Image..\n");

    xsize = (int)width;
    ysize = (int)height;

    fp = fopen(filename, "wb");
    if (fp==NULL){ return WSP_IMAGE_STATE_COULDNT_OPEN; }

    for (i=0; i<xsize*ysize; i++){
        if (im_max < in_u8[i]){ im_max = in_u8[i]; }
    }
    fprintf(fp, "P5\n%d %d\n%d\n", xsize, ysize, im_max);

    i = fwrite(in_u8, data_size, xsize*ysize, fp);
    if (i != xsize*ysize) { fclose(fp); return WSP_IMAGE_STATE_WRITING_FILE_FAILED; }

    fclose(fp);

    return WSP_IMAGE_STATE_SUCCESS;    
}


