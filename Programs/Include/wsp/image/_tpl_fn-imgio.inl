/**
 * @file _tpl_fn-imgio.inl
 * 
 */

#ifndef WspImage_WspFnImgIO_tpl_INL_
#define WspImage_WspFnImgIO_tpl_INL_

#include <wsp/common/ScopedFileOpen.h>

#define NUM_OF_HEADER_PARAM 6

//template<typename T>
//wsp::State wsp::img::LoadImageFile(T **o_img, int &o_width, int &o_height, int &o_num_channels, const char *in_filename){
//    char extension[MAX_LENGTH_OF_NAME];
//    wsp::GetExtension(extension, in_filename);
//    wsp::img::ImageFormat img_fmt = wsp::img::GetImageFormatID(extension);
//
//    switch(img_fmt){
//    case WSP_IMAGE_FORMAT_ADRI:
//        break;
//    default:
//
//        break;
//    }
//
//
//    return WSP_STATE_SUCCESS;
//}
//#ifdef USE_DEVIL
//inline wsp::State wsp::img::ilSaveImage(const uchar *in_img, const uchar *in_alphaData, u32 width, u32 height, u32 nChannel, const char *filename){
//    //u32 dataLen = height*width;
//    //wsp::State st;
//
//    //u32 totalChannel = (nChannel+1);
//    //uchar *tmp_img = new uchar[dataLen * totalChannel];
//    //MergeChannels(tmp_img, totalChannel, in_img, nChannel, in_alphaData, width, height);    //! Combine with alpha
//    //st = wsp::img::ilSaveImage(in_img, width, height, totalChannel, filename);
//    //if(st==WSP_STATE_FAILURE){ delete[] tmp_img; return WSP_STATE_FAILURE; }
//    //delete[] tmp_img;
//    return WSP_STATE_SUCCESS;
//}
//inline wsp::State wsp::img::ilSaveImage8(const uchar *in_img, u32 width, u32 height, const char *filename){
//    //return wsp::img::ilSaveImage(in_img, width, height, 1, filename);
//    return WSP_STATE_SUCCESS;
//}
//inline wsp::State wsp::img::ilSaveImage8(const uchar *in_img, const uchar *in_alphaData, u32 width, u32 height, const char *filename){
//    //u32 dataLen = height*width;
//    //wsp::State st;
//    //uchar *tmp_imgRgba = new uchar[dataLen * 4];
//    //MergeChannels(tmp_imgRgba, 4, in_img, 1, in_alphaData, width, height);    //! Convert RGB to RGBA
//    //st = wsp::img::ilSaveImage(tmp_imgRgba, width, height, 4, filename);
//    ////st = wsp::img::ilSaveImage(in_img, width, height, 1, filename);
//    //if(st==WSP_STATE_FAILURE){ delete[] tmp_imgRgba; return WSP_STATE_FAILURE; }
//    //delete[] tmp_imgRgba;
//    return WSP_STATE_SUCCESS;
//}
//inline wsp::State wsp::img::ilSaveImage24(const uchar *in_img, u32 width, u32 height, const char *filename){
//    //return wsp::img::ilSaveImage(in_img, width, height, 3, filename);
//    return WSP_STATE_SUCCESS;
//}
//inline wsp::State wsp::img::ilSaveImage24(const uchar *in_img, const uchar *in_alphaData, u32 width, u32 height, const char *filename){
//    //u32 dataLen = height*width;
//    //wsp::State st;
//    //uchar *tmp_imgRgba = new uchar[dataLen * 4];
//    //MergeChannels(tmp_imgRgba, 4, in_img, 3, in_alphaData, width, height);    //! Convert RGB to RGBA
//    //st = wsp::img::ilSaveImage(tmp_imgRgba, width, height, 4, filename);
//    ////st = wsp::img::SaveImage(in_img, width, height, 3, filename);
//    //if(st!=WSP_STATE_SUCCESS){ delete[] tmp_imgRgba; return WSP_STATE_FAILURE; }
//    //delete[] tmp_imgRgba;
//    return WSP_STATE_SUCCESS;
//}
//#endif

//! Raw data I/O ---------------------------------------------------
template<typename T> 
wsp::State wsp::img::SaveRawData(const T *in_data, 
                                const uchar *in_alpha, const uchar *in_state, 
                                int width, int height, int nChannels, const char *in_fileNameFormat, ...){
    char filename[MAX_LENGTH_OF_NAME];
    va_list ap;
    va_start(ap, in_fileNameFormat);
    vsprintf(filename, in_fileNameFormat, ap);
    va_end(ap);

    FILE *fp;
    size_t size;
    u32 len, lendata;
    fp = fopen(filename, "wb");
    if (fp==NULL){
        WSP_COMMON_ERROR_LOG("Opening %s failed\n", filename);
        return WSP_STATE_FAILURE;
    }
    fprintf(fp, "%zu %d %d %d %d %d\n", sizeof(T), width, height, nChannels, 
                                    in_alpha==NULL? 0:1, in_state==NULL? 0:1);
    len=width*height;
    lendata = len*nChannels;
    if(in_data!=NULL){
        size = fwrite(in_data, sizeof(T), lendata, fp);
        if (size != lendata) { fclose(fp); WSP_COMMON_ERROR_LOG("fwrite failed\n"); return WSP_STATE_FAILURE; }
    }
    if(in_alpha!=NULL){
        size = fwrite(in_alpha, sizeof(uchar), len, fp);
        if (size != len) { fclose(fp); WSP_COMMON_ERROR_LOG("fwrite failed\n"); return WSP_STATE_FAILURE; }
    }
    if(in_state!=NULL){
        size = fwrite(in_state, sizeof(uchar), len, fp);
        if (size != len) { fclose(fp); WSP_COMMON_ERROR_LOG("fwrite failed\n"); return WSP_STATE_FAILURE; }
    }    
    fclose(fp);
    printf("%s successfully saved as raw data\n", filename);
    return WSP_STATE_SUCCESS;
}

template<typename T>
wsp::State wsp::img::LoadRawData(T **o_data, 
                                 uchar **o_alpha, uchar ** o_state, 
                                 int &o_width, int &o_height, int &o_nChannels, const char *fileName)
{
    unsigned char line[70], c;
    int i, num_read = 0, num_max =6;
    int num[NUM_OF_HEADER_PARAM]; //! parameter container
    u32 len, lendata;
    wsp::common::ScopedOpenFile scoped_open(fileName, "rb");
    FILE *fp = scoped_open.GetOpenedFilePointer();

    int read_alpha, read_state;
    if(fp==NULL)
    {
        WSP_COMMON_ERROR_LOG("Opening %s failed\n", fileName);
        return WSP_STATE_FAILURE;
    }

    //! initialize nums
    for(i=0; i<6; i++){ num[i] = -1; }

    fgets((char *)line, sizeof(line), fp);

    for (i=0; c=line[i]; i++)
    {
        if (c == '#') break;
        if (isdigit(c))
        {
            if (num_read >= num_max)
            {
                return WSP_STATE_FAILURE;
            }
            num[num_read] = c - '0';
            for (; c=line[i+1]; i++)
            {
                if (!isdigit(c)){ break; }
                num[num_read] = 10*num[num_read] + c - '0';
            }
            ++num_read;
        }
    }
    if(num_read!=NUM_OF_HEADER_PARAM)
    {
        WSP_COMMON_ERROR_LOG("Number of params in header is wrong: %d\n", num_read);
        return WSP_STATE_FAILURE;
    }
    if(num[0]!=sizeof(T))
    {
        WSP_COMMON_ERROR_LOG("Element size is wrong: file data size=%d, current data size=%zu\n", num[0], sizeof(T));
        return WSP_STATE_FAILURE;
    }
    o_width = num[1];
    o_height = num[2];
    o_nChannels = num[3];
    len = o_width*o_height;
    lendata = o_width*o_height*o_nChannels;
    read_alpha = num[4];
    read_state = num[5];

    printf("elem size %zu, width %d, height %d, num of channels %d, read alpha %d, read state %d\n",
            sizeof(T), o_width, o_height, o_nChannels, read_alpha, read_state);

    size_t cnt;
    if(o_data!=NULL)
    {
        wsp::ReallocArrayMem<T>(o_data, lendata);
        cnt = fread(*o_data, sizeof(T), lendata, fp);
        if( cnt != lendata)
        {
            WSP_COMMON_ERROR_LOG("Reading data failed %zu!=%u\n", cnt, lendata);
            return WSP_STATE_FAILURE;
        }
        //printf("data is loaded, ");
    }
     if(o_alpha!=NULL ){
        wsp::ReallocArrayMem<uchar>(o_alpha, len);
        if(read_alpha==1){
            cnt = fread(*o_alpha, sizeof(uchar), len, fp);
            if( cnt != len)
            {
                WSP_COMMON_ERROR_LOG("Reading alpha data failed %zu!=%u\n", cnt, len);
                return WSP_STATE_FAILURE;
            }
            //printf("alpha is loaded, ");
        }else{
            memset(*o_alpha, 0xff, len);
        }
    }
     if(o_state!=NULL){
        wsp::ReallocArrayMem<uchar>(o_state, len);
        if(read_state==1){
            cnt = fread(*o_state, sizeof(uchar), len, fp);
            if( cnt != len)
            {
                WSP_COMMON_ERROR_LOG("Reading state data failed %zu!=%u\n", cnt, len);
                return WSP_STATE_FAILURE;
            }
            //printf("state is loaded\n");
        }else{
            memset(*o_state, 0x00, len);
        }
    }
    printf("%s successfully loaded\n", fileName);
    return WSP_STATE_SUCCESS;
}



//! Original Format ----------------------------------------------
//! Adaptive Dynamic Range Image
template<typename T>
wsp::State wsp::img::CompressInADR(int *o_bin, const T *in_src, int length, int dynamic_range_bit, T im_min)
{
    int i, j;
    //std::cout << "mem_size = " << mem_size << endl;

    //! setting all bit to 0
    //for (i=0; i<mem_size / sizeof(int); i++) b[i] = 0;


    // compress and store data
    int pointer = 0;
    int counter = 0;
    o_bin[0] = 0;
    int mask;
    u32 extracted_bit; //! unsigned int can do 0-filled-right-shift(>>)
    int cue_size = sizeof(int) * 8;
    for(i=0; i<length; i++)
    {
        //mask = 1 << dynamic_range_bit-1;
        int value = static_cast<int>(in_src[i]-im_min);
        //negative_bit = value & mask;
        //if(negative_bit==1){
        //    //! make value negative. 
        //    //! for instance, if dynamic range bit is 3, than change 0110 to 1110
        //    value <<= cue_size - dynamic_range_bit; 
        //    value >>= cue_size - dynamic_range_bit; 
        //}
        //printf("%d: %d\n", i, value);
        //printf("%d: \n", i);

        for(j=dynamic_range_bit-1; j>=0; j--)
        {
            // ---------------------------------------
            //! extract a target bit into tmp in order.
            //! for example, if dynamic_range_bit = 3 
            //! than 00000101 is extracted like following
            //! first loop -> 00000001, 
            //! second loop-> 00000000, 
            //! third loop -> 00000001
            mask = 1 << j; //! if dynamic_range_bit=3, than 100
            extracted_bit = value & mask; //! if color_bit=3 and value=101, than 101 & 100 = 100

            //! shift bit to top 
            extracted_bit >>= j; //! if color_bit=3 and value=101, than 001
            // ---------------------------------------

            //! write 1 bit and increment counter
            extracted_bit <<= counter;
            o_bin[pointer] |= extracted_bit;
            counter++;

            //printf("%x ", tmp);

            //wsp::PrintAsBit(bin[pointer]);
            //printf("\n");
            //! if 4 byte filled, go to next byte, and initialize counter
            if(counter >= cue_size){
                pointer++;
                counter = 0;
                o_bin[pointer]=0;
                continue;
            }

            // shift 1 bit to make space for storing next data
            //bin[pointer] <<= 1;

        }
        //printf("\n");
    }


    return WSP_STATE_SUCCESS;
}

template<typename T> 
wsp::State wsp::img::DecompressADR(T *o_img, const int *in_src, int length, int dynamic_range_bit, int min)
{
    int i, j;
    int value;

    int mask;
    int cue = in_src[0];
    int cue_size = sizeof(int) * 8;
    int counter = 0;
    int pointer = 0;
    u32 extracted_bit; //! unsigned int can do 0-filled-right-shift(>>)
    for(i=0; i<length; i++)
    {
        value = 0;
        //printf("%d: \n", i);
        for(j=dynamic_range_bit-1; j>=0; j--)
        {
            //! read 1 bit and increment counter
            mask = 1 << counter; //(cuesize - 1 - counter);
            extracted_bit = cue & mask;

            //! shift bit to top
            extracted_bit >>= counter;//cuesize - 1 - counter;
            //printf("%x ", extracted_bit);

            extracted_bit <<= j;
            value |= extracted_bit;
            counter++;
            //! if 1 byte read, go to next byte, and initialize counter
            if(counter >= cue_size)
            {
                pointer++;
                cue = in_src[pointer];
                counter = 0;
                //continue;
            }
            //wsp::PrintAsBit(value);
            //printf("\n");


        }
        o_img[i] = static_cast<T>(value + min);
        //printf("%d: %d\n", i, o_img[i]);
        //printf("\n");
    }

    return WSP_STATE_SUCCESS;
}


template<typename T>
wsp::State wsp::img::SaveAsADRI(
    const T *in_img, 
    int width, int height, int num_of_channels, 
    const char* filename)
{
    int i;
    T im_max, im_min;
    int min, max;
    int xsize, ysize, nc, len;
    //int data_size;
    int dynamic_range_bit;
    int color_range;
    int mem_size, mem_bit;
    int *bin;

    int spare_mem = 128;

    //! initializing variables
    xsize = (int)width;
    ysize = (int)height;
    nc = num_of_channels;

    len = xsize*ysize*nc;
    //data_size = sizeof(uchar);

    //std::cout << "imSaveCompress=========================" << endl;
    std::printf("Saving %s as ADRI\n", filename);


    //! searching max and min value
    im_min = in_img[0];
    im_max = in_img[0];
    for (i=0; i<len; i++){
        if(im_min > in_img[i]){ im_min = in_img[i]; }
        if(im_max < in_img[i]){ im_max = in_img[i]; }
    }
    min = static_cast<int>(im_min);
    max = static_cast<int>(im_max);
    color_range = max - min + 1;

    if(color_range > pow(2.0, 32)){
        WSP_COMMON_ERROR_LOG("The image compression is not apply to image which has more than 32 bit intensities\n");
        return WSP_STATE_FAILURE;
    }

    //! calculating color bit for this image
    dynamic_range_bit = 1;
    while( color_range > (1 << dynamic_range_bit) ){ dynamic_range_bit++; }

    //dynamic_range_bit=8;
    mem_bit = len * dynamic_range_bit;
    mem_size = (mem_bit + 7) / 8;

    wsp::common::ScopedOpenFile scoped_open(filename, "wb");
    FILE *fp = scoped_open.GetOpenedFilePointer();
    if(!fp)
    {
        WSP_COMMON_ERROR_LOG("opening file failed\n");
        return WSP_STATE_FAILURE;
    }

    fprintf(fp, "ADRI\n%d %d %d\n%d\n%d %d\n", xsize, ysize, nc, dynamic_range_bit, min, max);
    printf("    width: %d, height: %d, num of channels: %d, color bit: %d\n", xsize, ysize, nc, dynamic_range_bit);
    printf("    min: %d, max: %d\n", min, max);
    printf("    mem size: %d byte\n", mem_size);

    bin = reinterpret_cast<int *>(malloc(mem_size + spare_mem));

    wsp::img::CompressInADR(bin, in_img, len, dynamic_range_bit, im_min);
    i = (s32)fwrite(bin, 1, (u32)mem_size, fp);
    if(i != mem_size){
        std::free(bin);
        return WSP_STATE_FAILURE;
    }
    
    std::free(bin);

    return WSP_STATE_SUCCESS;
}

template<typename T>
wsp::State wsp::img::LoadADRI(T **o_img, int &o_width, int &o_height, int &o_num_of_channels,
                                                   const char *filename)
{
    char LINE[70], c;
    int i, type_read, num_read, num_max;
    int num[6];
    int len, dynamic_range_bit, min, max;
    int data_size;

    //! initializing variables
    data_size = sizeof(uchar);
    type_read = 0;
    num_read = 0;
    num_max = 6; //! number of header property

    
    printf("Loading %s as ADRI\n", filename);


    //! opening file
    wsp::common::ScopedOpenFile scoped_open(filename, "rb");
    FILE *fp = scoped_open.GetOpenedFilePointer();
    if(!fp)
    {
        WSP_COMMON_ERROR_LOG("%s doesn't exist\n", filename);
        return WSP_STATE_FAILURE;
    }

    //! reading header
    while (fgets((char *)LINE, sizeof(LINE), fp))
    {
        i = 0;

        if (!type_read)
        {
            if(!strcmp(LINE, "ADRI"))
            {
                if(data_size!=sizeof(uchar))
                {
                    return WSP_STATE_FAILURE;
                }
                num_max = 2;
            }
            type_read = 1;
        }

        for (; c=LINE[i]; i++)
        {
            if (c == '#') break;
            if (isdigit(c))
            {
                if (num_read >= num_max)
                {
                    WSP_COMMON_ERROR_LOG("invalid header\n");
                    return WSP_STATE_INVALID_PARAMETER;
                }
                num[num_read] = c - '0';
                for (; c=LINE[i+1]; i++)
                {
                    if (!isdigit(c)) break;
                    num[num_read] = 10*num[num_read] + c - '0';
                }
                num_read ++;
            }
            else if(c == '-')
            {
                if(num_read >= num_max)
                {
                    WSP_COMMON_ERROR_LOG("invalid header\n");
                    return WSP_STATE_INVALID_PARAMETER;
                }

                if ( !isdigit( c=LINE[++i] ) ){ break; }

                num[num_read] = c - '0';
                for (; c=LINE[i+1]; i++)
                {
                    if (!isdigit(c)) break;
                    num[num_read] = 10*num[num_read] + c - '0';
                }
                num[num_read] = -num[num_read];
                num_read ++;
            }
        }

        if (num_read >= num_max) break;
    }
    if(num_read != num_max)
    {
        WSP_COMMON_ERROR_LOG("invalid header\n");
        return WSP_STATE_INVALID_PARAMETER;
    }

    o_width = num[0];
    o_height = num[1];
    o_num_of_channels = num[2];
    len = o_width*o_height*o_num_of_channels;
    dynamic_range_bit = num[3];
    min = num[4];
    max = num[5];

    printf("    width: %d, height: %d, num of channels: %d, color bit: %d\n", o_width, o_height, o_num_of_channels, dynamic_range_bit);
    printf("    min: %d, max: %d\n", min, max);

    //std::cout<< "o_width = "<<o_width<<",o_height = "<<o_height<<endl;
    //std::cout<< "dynamic_range_bit = "<<dynamic_range_bit<<",min = "<<min<<",max = "<<max<<endl;
    //delete[] o_img;
    //o_img = new uchar[len];
    wsp::ReallocArrayMem<T>(o_img, len);
    if(!o_img)
    {
        WSP_COMMON_ERROR_LOG("Not enough memory\n");
        return WSP_STATE_INSUFFICIENT_MEMORY;
    }


    //! initializing image
    for(i=0; i<len; i++){ (*o_img)[i]=0; }


    //! decoding image
    int mem_size = ( len * dynamic_range_bit + 7) / 8;


    int *bin = reinterpret_cast<int*>(malloc(mem_size));
    if (bin==NULL)
    {
        WSP_COMMON_ERROR_LOG("allocating memory failed\n");
        return WSP_STATE_INSUFFICIENT_MEMORY;
    }

    if (fread(bin, 1, mem_size, fp) != (size_t)mem_size)
    {
        free(bin);
        WSP_COMMON_ERROR_LOG("Can't read compressed data\n");
        return WSP_STATE_FAILURE;
    }
    
    wsp::img::DecompressADR(*o_img, bin, len, dynamic_range_bit, min);

    free(bin);

    printf( "The ADRI was successfully loaded.\n" );

    return WSP_STATE_SUCCESS;
}

#endif
