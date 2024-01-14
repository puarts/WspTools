// @file wspimghandler.h
// 
// Description: Base class for any image processing class, which can manage the I/O of images

#ifndef __WSP_IMAGE_CLASS_IMAGE_HANDLER_H__
#define __WSP_IMAGE_CLASS_IMAGE_HANDLER_H__

#include "tpl_cl-image.h"

namespace wsp{
    class WSP_DLL_EXPORT ImageHandler
    {
    public:
        static wsp::DebugMode dbgmode;

    protected:
        int num_image_8u_, num_image_64f_; // number of images
        char dest_dir_[MAX_LENGTH_OF_PATH];
        char dest_name_[MAX_LENGTH_OF_NAME];
        wsp::SeqInfo seq_info_8u_;
        wsp::SeqInfo seq_info_64f_;
        wsp::Image<uchar> *image_8u_;
        wsp::Image<double> *image_64f_;

    protected:
        inline void Init();

    public:
        ImageHandler();
        ImageHandler(const char *filename_col, const char *filename_depth);
        ImageHandler(int num_image_8u, int num_image_32f);
        ~ImageHandler();

        // Getter
        inline wsp::Image<uchar>* image_8u(int index) const;
        inline wsp::Image<double>* image_64f(int index) const;
        inline wsp::SeqInfo seq_info_8u() const{ return seq_info_8u_; }
        inline wsp::SeqInfo seq_info_64f() const{ return seq_info_64f_; }

        // Setter
        void InitImage8u(int num_img);
        void InitImage32f(int num_img);
        inline void set_seq_info_8u(const char* filename);
        inline void set_seq_info_64f(const char* filename);
        inline void set_dest_dir(const char* destDir){ strcpy(dest_dir_, destDir); }
        inline void set_dest_name(const char* destName){ strcpy(dest_name_, destName); }

        // I/O
        inline void PrintSeqInfo8u() const{ std::cout<<seq_info_8u_; }
        inline void PrintSeqInfo32f() const{    std::cout<<seq_info_64f_; }
    };
}
// Getter --------------------------------------------------------
inline wsp::Image<uchar>* wsp::ImageHandler::image_8u(int index) const{
    if(index>=num_image_8u_){ return NULL; }
    return &(image_8u_[index]);
}

inline wsp::Image<double>* wsp::ImageHandler::image_64f(int index) const{
    if(index>=num_image_64f_){ return NULL; }
    return &(image_64f_[index]);
}
// Setter --------------------------------------------------------
inline void wsp::ImageHandler::set_seq_info_8u(const char* filename){
    wsp::GetSeqInfo(seq_info_8u_, filename);
}
inline void wsp::ImageHandler::set_seq_info_64f(const char* filename){
    wsp::GetSeqInfo(seq_info_64f_, filename);
}


#endif