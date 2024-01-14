// @file tpl_cl-layeredimage.h
// 

#ifndef _WSP_IMAGE_TEMPLATE_CLASS_LAYRED_IMAGE_H_
#define _WSP_IMAGE_TEMPLATE_CLASS_LAYRED_IMAGE_H_

//#include "WspImage_Index.h>
#include "tpl_cl-image.h"

namespace wsp{
    template<class T>
    class LayeredImage
    {
    private:
        u32 m_nLayers;
        u32 width_, height_, num_of_channels_;
        wsp::Image<T> *data_;

    public:
        LayeredImage();
        LayeredImage(u32 numOfLayers, u32 width, u32 height, u32 nChannels=1);
        LayeredImage(const LayeredImage &in_img);
        ~LayeredImage();

        // Getter --------------
        inline u32 NumOfLayers() const{ return m_nLayers; };
        inline u32 width() const{ return width_; }
        inline u32 height() const{ return height_; }
        inline u32 num_of_channels() const{ return num_of_channels_; }
        inline wsp::Image<T> GetLayer(u32 layerIndex) const{ return data_[layerIndex]; }
        inline wsp::Image<T>* data(){ return data_; }
        wsp::State GetCombinedImage(wsp::Image<T> &o_dst) const;

        // Setter --------------
        wsp::State SetSize(u32 numOfLayers, u32 width, u32 height, u32 nChannels);
        wsp::State CreateLayersFromSegmImage(const wsp::Image<T> &in_src, const wsp::Image<int> &in_segm);

        // I/O -----------------
        wsp::State SaveAllLayersSeparately(const char *filename) const;
        wsp::State SaveCombinedImage(const char *filename) const;

        #ifdef USE_QT
        wsp::State SaveAllLayersSeparatelyDialog() const;
        wsp::State SaveCombinedImageDialog() const;
        #endif

        // Operators -----------
        wsp::LayeredImage<T>& operator=(const wsp::LayeredImage<T>& other);
        bool operator==(const wsp::LayeredImage<T>& other) const;
        inline bool operator!=(const wsp::LayeredImage<T> &other) const{ return !(*this==other); }

        inline wsp::Image<T>& operator[](u32 index){ return data_[index]; }
        inline wsp::Image<T>& operator[](int index){ if(index<0){ return data_[0]; } return operator[]((u32)index); }
        inline const wsp::Image<T> operator[](u32 index) const{ return data_[index]; }
        inline const wsp::Image<T> operator[](int index) const{ if(index<0){ return data_[0]; } return operator[]((u32)index); }
    };
}

// Constructor, Destructor ----------------------------------------
#define INIT_LayeredImage data_(NULL), m_nLayers(0), width_(0), height_(0), num_of_channels_(1)

template<class T> wsp::LayeredImage<T>::LayeredImage()
    : INIT_LayeredImage
{}
template<class T> wsp::LayeredImage<T>::LayeredImage(u32 numOfLayers, u32 width, u32 height, u32 nChannels)
    : INIT_LayeredImage
{
    SetSize(numOfLayerss, width, height, nChannels);
}
template<class T> wsp::LayeredImage<T>::LayeredImage(const LayeredImage &in_img)
    : INIT_LayeredImage
{
    m_nLayers = in_img.NumOfLayers();
    width_ = in_img.width();
    height_ = in_img.height();
    num_of_channels_ = in_img.num_of_channels();
    data_ = WSP_NEW wsp::Image<T>[m_nLayers];
    for(u32 i=0; i<m_nLayers; i++){
        data_[i] = in_img[i];
    }
}
template<class T> wsp::LayeredImage<T>::~LayeredImage()
{
    delete[] data_;
}
// ---------------------------------------- end Constructor, Destructor


// Getter ----------------------------------------------------------
template<class T> wsp::State wsp::LayeredImage<T>::GetCombinedImage(wsp::Image<T> &o_dst) const
{
    u32 i, x, y, c, i_img, i_lay, ic_img, ic_lay;
    u32 a_img, a_lay, a_sum; //! temp var for alpha
    double r_img, r_lay; //! distribution ratio for img and layer

    o_dst.SetSize(width_, height_, num_of_channels_);
    o_dst.Reset(0);
    o_dst.ResetAlpha(0);
    for(i=0; i<m_nLayers; i++){
        //char name[MAX_LENGTH_OF_PATH];
        //sprintf(name, "a_layerCombCheck_%d.png", i);
        //o_dst.SaveImage(name);
        //sprintf(name, "a_layerCombCheck_layer_%d.png", i);
        //data_[i].SaveImage(name);
        //sprintf(name, "a_layerCombCheck_layer_alpha_%d.png", i);
        //data_[i].SaveAlpha(name);

        for(y=0; y<data_[i].height(); y++){
            for(x=0; x<data_[i].width(); x++){
                i_lay = y*data_[i].width() + x;
                i_img = y*o_dst.width() + x;

                //! calculate new pixel value
                a_img = (u32)o_dst.GetAlpha(i_img);
                a_lay = (u32)data_[i].GetAlpha(i_lay);
                a_sum = a_img + a_lay;
                if(a_sum>255){ a_sum = 255; }

                r_lay = (double)a_lay/(double)a_sum;
                r_img = (double)(a_sum - a_lay)/(double)a_sum;

                ic_img = i_img*num_of_channels_;
                ic_lay = i_lay*data_[i].num_of_channels();
                for(c=0; c<num_of_channels_; c++){
                    if(c<data_[i].num_of_channels()){
                        o_dst[ic_img+c] = (T)(o_dst[ic_img+c]*r_img + data_[i][ic_lay+c]*r_lay);
                        continue;
                    }
                    o_dst[ic_img+c] = (T)(o_dst[ic_img+c]*r_img + data_[i][ic_lay+c]*r_lay);
                }
                o_dst.SetAlpha(a_sum, i_img);
            }
        }
    }
    return WSP_STATE_SUCCESS;
}

// ---------------------------------------------------------- end Getter

// Setter ----------------------------------------------------------
template<class T> wsp::State wsp::LayeredImage<T>::SetSize(u32 numOfLayers, u32 width, u32 height, u32 nChannels)
{
    u32 nLayers_old = m_nLayers;
    LayeredImage img_old(*this);

    m_nLayers = numOfLayers;
    width_ = width;
    height_ = height;
    num_of_channels_ = nChannels;
    delete[] data_;
    data_ = WSP_NEW wsp::Image<T>[m_nLayers];

    for(u32 i=0; i<m_nLayers; i++){
        if(i<nLayers_old){
            data_[i] = img_old[i];
            data_[i].SetSize(width_, height_, num_of_channels_);
            continue;
        }
        data_[i].SetSize(width_, height_, num_of_channels_);
    }
    return WSP_STATE_SUCCESS;
}
template<class T>
wsp::State wsp::LayeredImage<T>::CreateLayersFromSegmImage(const wsp::Image<T> &in_src, const wsp::Image<int> &in_segm)
{
    u32 w=in_segm.width(), h=in_segm.height(), nc=in_src.num_of_channels();
    if(w!=in_src.width() || h!=in_src.height())
    { WSP_COMMON_ERROR_LOG("sizes of input image and segmentation image must be the same\n"); return WSP_STATE_FAILURE; }

    u32 len = w*h;
    int i_seg, nSegm;
    u32 segm, i, ic, c;
    wsp::Vector<int> segmList;

    in_segm.GetIndexList(segmList);
    nSegm = segmList.length();

    m_nLayers = nSegm;
    width_ = w;
    height_ = h;
    num_of_channels_ = nc;
    delete[] data_;
    data_ = WSP_NEW wsp::Image<T>[m_nLayers];

    //! iterate all segmentation
    for(i_seg=0; i_seg<nSegm; i_seg++){
        segm = segmList[i_seg];
        data_[i_seg].SetSize(w, h, nc);

        //! creating layer according to segm image
        data_[i_seg].Reset(0);
        data_[i_seg].ResetAlpha(0);
        for(i=0; i<len; i++){
            if(in_segm[i]==segm){
                ic = i*nc;
                for(c=0; c<nc; c++){
                    data_[i_seg][ic+c] = in_src[ic+c];
                }
                data_[i_seg].SetAlpha(255, i);
            }
        }
    }

    return WSP_STATE_SUCCESS;
}

// ---------------------------------------------------------- end Setter

// I/O --------------------------------------------------------------------
template<class T>
wsp::State wsp::LayeredImage<T>::SaveAllLayersSeparately(const char *in_filepath) const
{
    if(m_nLayers==0){ return WSP_STATE_SUCCESS; }
    char filepath[MAX_LENGTH_OF_PATH], dir[MAX_LENGTH_OF_PATH], filename[MAX_LENGTH_OF_NAME], name[MAX_LENGTH_OF_NAME], ext[MAX_LENGTH_OF_NAME];
    wsp::State wstat;
    wsp::GetDir(dir, in_filepath);
    wsp::GetFilename(filename, in_filepath);
    wsp::GetName(name, filename);
    wsp::GetExtension(ext, filename);
    printf("SaveAllLayersSeparately-----------\n");
    printf("dir: %s\nfilename: %s\nname: %s\next: %s\n", dir, filename, name, ext);
    for(u32 i=0; i<m_nLayers; i++){
        sprintf(filepath, "%s/%s_%d.%s", dir, name, i, ext);
        wstat = data_[i].SaveImage(filepath);
        if(wstat == WSP_STATE_FAILURE){ WSP_COMMON_ERROR_LOG("Saving %s failed", filepath); return WSP_STATE_FAILURE; }
    }
    return WSP_STATE_SUCCESS;
}
template<class T>
wsp::State wsp::LayeredImage<T>::SaveCombinedImage(const char *filename) const{
    wsp::Image<T> tmp_img;
    GetCombinedImage(tmp_img);
    return tmp_img.SaveImage(filename);
}

#ifdef USE_QT
template<class T>
wsp::State wsp::LayeredImage<T>::SaveAllLayersSeparatelyDialog() const{
    QString selectedFilter;
    QString qfilename = QFileDialog::getSaveFileName(NULL, QString("Save As"),
                            QDir::currentPath(), wsp::formatFilterImg, &selectedFilter);
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    return SaveAllLayersSeparately(qfilename.toLatin1().data());
}
template<class T>
wsp::State wsp::LayeredImage<T>::SaveCombinedImageDialog() const{
    wsp::Image<T> tmp_img;
    GetCombinedImage(tmp_img);
    return tmp_img.SaveImageDialog();
}
#endif

// -------------------------------------------------------------------- end I/O 


// Operators ---------------------------------------------------------------------------------
template<class T> wsp::LayeredImage<T>& wsp::LayeredImage<T>::operator=(const wsp::LayeredImage<T>& other){
    if(this != &other){
        if(other.NumOfLayers() != 0){
            SetSize(other.NumOfLayers());
            for(u32 itr_i=0; itr_i<m_nLayers; itr_i++){
                data_[itr_i] = other[itr_i];
            }
        }
    }
    return *this;
}
template<class T> bool wsp::LayeredImage<T>::operator==(const wsp::LayeredImage<T> &other) const{
    if(other.data()==NULL && data_==NULL && m_nLayers==other.NumOfLayers()){
        return true;
    }
    if(other.NumOfLayers()!=m_nLayers || data_==NULL || other.data()==NULL){
        return false;
    }
    for(u32 itr_i=0; itr_i<m_nLayers; itr_i++){
        if(data_[itr_i]!=other.GetLayer(itr_i)){
            return false;
        }
    }
    return true;
}



#endif