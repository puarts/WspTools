//! _fn-videoproc.h
// @author Jun-ichi Nishikata
//! video processing library

#ifndef _WSP_FN_VIDEOPROC_H_
#define _WSP_FN_VIDEOPROC_H_

#include <stdio.h>

#include <wsp/image/tpl_cl-imageseq.h>
#include <wsp/image/fn-imgproc.h>
#include <wsp/image/gpl_tpl_fn-imgproc.h>
#include "_define_video.h"
#include "cl-opticalflowdata.h"

#ifdef USE_OPENMAYA
#include <wsp/maya/_define_maya.h>
#endif


namespace wsp{ namespace video{
    template<typename T, typename U>
    wsp::State GetOpticalFlowBM(
        wsp::ImageSeq<T> *o_dst_raw_seq,
        wsp::ImageSeq<U> *o_dst_norm_seq, 
        const wsp::ImageSeq<U> &in_src_seq, 
        const wsp::img::StereoBMParams &params,
        int frame_offset, 
        wsp::video::OpticalflowDisplayType out_disp_type,
        bool save_norm_data, 
        bool save_raw_data
#ifdef USE_QT
        , QLabel *image_label, QProgressBar *progress_bar
#endif
    );

    // -----------------------------------------------------------------------------------
    //! extract only continuous opticalflow from opticalflow map sequence
    //!
    //!   o_optf_map: the output sequence draw points with any color for each optical flows
    //!   o_optf_list: the output optical flow list
    //!
    //!   end_frame=-1 means end_frame is to find opticalflow to last frame in the sequecne
    template<typename T>
    wsp::State ExtractContinuousOpticalflowList(
        const wsp::ImageSeq<T> &in_optf_map,
        wsp::OpticalflowList *o_optf_list, 
        //wsp::ImageSeq<T> *o_opft_map,
        int start_frame=0, 
        int end_frame=-1, 
        int pix_step=1
    );

    //! Draw optical flow points by random color
    template<typename T>
    wsp::State DrawOpticalflow(const wsp::OpticalflowList &in_optf_list, wsp::ImageSeq<T> &io_image);



}}





//! =========================================================================================
//! =========================================================================================
//!
//!                                  Optical flow 
//!
//! =========================================================================================
//! =========================================================================================
template<typename T, typename U>
wsp::State wsp::video::GetOpticalFlowBM(wsp::ImageSeq<T> *o_dst_raw_seq,
                                        wsp::ImageSeq<U> *o_dst_norm_seq, 
                                        const wsp::ImageSeq<U> &in_src_seq, 
                                        const wsp::img::StereoBMParams &params,
                                        int frame_offset, 
                                        wsp::video::OpticalflowDisplayType out_disp_type,
                                        bool save_norm_data, bool save_raw_data
#ifdef USE_QT
                                        , QLabel *image_label, QProgressBar *progress_bar
#endif
                                        )
{
    //wsp::Image<uchar> img_left, img_right;
    wsp::Image<uchar> img_left,img_right;
    wsp::Image<double> img_disp;
    wsp::State state;
    wsp::SeqInfo info;
    char filename[MAX_LENGTH_OF_PATH];
    char filename_alpha[MAX_LENGTH_OF_PATH];

    int start_frame = in_src_seq.SeqInfoSrc().start_frame;

    int seq_size = static_cast<int>(in_src_seq.SeqSize()) - frame_offset;
    if(o_dst_raw_seq!=NULL){  o_dst_raw_seq->SetSeqSize(seq_size); }
    if(o_dst_norm_seq!=NULL){ o_dst_norm_seq->SetSeqSize(seq_size); }
    int end_frame = start_frame + seq_size;

    #ifdef USE_QT
    if(progress_bar!=NULL){
        progress_bar->setRange(0, seq_size); progress_bar->setValue(0);
    }
    #endif
    for(int i=0, frame=start_frame; i<seq_size; i++, frame++)
    {
        printf("\nprocessing %d frame ----------------------------------\n", frame);
        #ifdef USE_QT
        if(progress_bar!=NULL){ progress_bar->setValue(i); }
        qApp->processEvents();
        #endif

        in_src_seq[i].CopyIntoAnyImage(img_left);
        in_src_seq[i+frame_offset].CopyIntoAnyImage(img_right);

        state=wsp::img::StereoBM(img_disp, img_left, img_right, params);
        if(state!=WSP_STATE_SUCCESS){ 
            #ifdef USE_QT
            if(progress_bar!=NULL){ progress_bar->setValue(0); }
            #endif
            WSP_COMMON_ERROR_LOG("Stereo matching failed\n"); return state; 
        }
        img_disp.ResetAlpha(1);
        img_disp.SetAlphaAtTargetStatePix(0, WSP_PIXEL_STATE_OCCLUDED);

        //! saving raw data
        if(save_raw_data){
            //sprintf(filename, "disp_raw.%04d.wspid", frame);
            //img_disp.SaveRawData(filename);
            sprintf(filename, "%s.%04d.adri", in_src_seq.SeqInfoSrc().name, frame);
            sprintf(filename_alpha, "%s_alpha.%04d.adri", in_src_seq.SeqInfoSrc().name, frame);
            img_disp.SaveAsADRI(filename, filename_alpha);
        }
        img_disp.NormalizeAlpha(0, 255);
        if(o_dst_raw_seq!=NULL){
            (*o_dst_raw_seq)[i] = img_disp;
        }
        switch(out_disp_type){
        case wsp::video::OPTICALFLOW_DISPLAY_SCALAR_XY: 
            img_disp.Normalize(0, 255);
            //img_disp.CombineChannelsByNorm(); 
            img_disp.CombineChannelsBySum(); 
            break;
        case wsp::video::OPTICALFLOW_DISPLAY_X: img_disp.LeaveOneChannel(0); break;
        case wsp::video::OPTICALFLOW_DISPLAY_Y: img_disp.LeaveOneChannel(1); break;
        default: break;
        }

        img_disp.Normalize(0, 255);
        //printf("///////////// num of channel:%d\n", img_disp.num_of_channels());
        //info = in_src_seq.SeqInfoSrc();
        //sprintf(filename, "%s/%s_disp.%4d.%s", info.dir, info.name, frame, info.ext);
        if(o_dst_norm_seq!=NULL){
            //(*o_dst_norm_seq)[frame].CopyFrom(img_disp, o_dst_norm_seq->num_of_channels());
            img_disp.CopyIntoAnyImage((*o_dst_norm_seq)[i], in_src_seq.num_of_channels());
        }
        if(save_norm_data){
            sprintf(filename, "%s.%04d.png", in_src_seq.SeqInfoSrc().name, frame);
            img_disp.SaveImage(filename);
        }
        #ifdef USE_QT
        if(image_label!=NULL){
            img_disp.SetAlphaAtTargetStatePix(0, WSP_PIXEL_STATE_OCCLUDED);
            //img_disp.CopyIntoAnyImage(in_src_seq[frame], in_src_seq.num_of_channels());
            wsp::img::qtWspImageToQLabel(*image_label, in_src_seq[i]);
        }
        #endif
    }
    #ifdef USE_QT
    if(progress_bar!=NULL){ progress_bar->setValue(0); }
    #endif
    return WSP_STATE_SUCCESS;
}

template<typename T>
wsp::State wsp::video::ExtractContinuousOpticalflowList(const wsp::ImageSeq<T> &in_optf_map, 
                                                        wsp::OpticalflowList *o_optf_list, 
                                                        //wsp::ImageSeq<T> *o_optf_map, 
                                                        int start_frame, 
                                                        int end_frame, 
                                                        int pix_step)
{
    if(start_frame>end_frame && end_frame!=-1){
        WSP_COMMON_ERROR_LOG("Invalid start_frame or end_frame\n"); return WSP_STATE_INVALID_PARAMETER;
    }
    int w = in_optf_map.width();
    int h = in_optf_map.height();
    int nc= in_optf_map.num_of_channels();
    int len = w*h;
    int seq_size = in_optf_map.SeqSize();
    int end = end_frame < 1? seq_size : end_frame; 
    if(seq_size<2 || len==0 || nc==0){
        WSP_COMMON_ERROR_LOG("Opticalflow map size is not enough\n"); return WSP_STATE_INVALID_PARAMETER;
    }

    printf("Opticalflow source data info: \n");
    printf("w=%d, h=%d, nc=%d, seq_size=%d\n", w, h, nc, seq_size);

    if(o_optf_list!=NULL){
        o_optf_list->SetLength(len);
    }
    //if(o_optf_map!=NULL){
    //    o_optf_map->CopyFrom(in_optf_map);
    //}

    int optf_len=0;
    int cnt = 0;


    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        double px, py, pz;
        double optf_x, optf_y;
        int x, y, i, i_d, i_nc, i_d_nc, frame;
        T r, g, b;
        int num_threads = 1;
#ifdef _OPENMP
        num_threads = omp_get_num_threads();
#endif
        float percentage_mult = pix_step*pix_step*100*num_threads/static_cast<float>(len);


        #ifdef _OPENMP
        #pragma omp for reduction(+:optf_len, cnt)
        #endif
        for(y=0; y<h; y+=pix_step){
            #ifdef USE_QT
            qApp->processEvents();
            #endif
            for(x=0; x<w; x+=pix_step){
                i = y*w + x;
                #ifdef USE_OPENMAYA
                wsp::maya::Printf("(y, x)=(%d, %d)\n", y, x);
                #else
                //printf("(y, x)=(%d, %d), ", y, x);
                cnt++;
                printf("thread %d: %5f %% done..\r", omp_get_thread_num(), cnt*percentage_mult);
                #endif
                uchar alpha = in_optf_map[start_frame].GetAlpha(i);
                if(alpha<0xff){ continue; }
                //printf("alpha = %d\n", alpha);
                if(o_optf_list!=NULL){
                    (*o_optf_list)[optf_len].SetLength(seq_size);
                }

                frame = start_frame;

                //! determine color of optical flow ---------------
                r = static_cast<T>(rand()*255 /RAND_MAX);
                g = static_cast<T>(rand()*255 /RAND_MAX);
                b = static_cast<T>(rand()*255 /RAND_MAX);

                //! add first optical flow ------------------------
                i_nc = i*nc;
                optf_x = in_optf_map[frame][i_nc];
                optf_y = in_optf_map[frame][i_nc+1];
                px = x + optf_x;
                py = y + optf_y;
                //! define z as magnitude of optical flow instead of actual depth
                pz = (optf_x*optf_x + optf_y*optf_y) * (optf_x*optf_y/abs(optf_x*optf_y));
                if(o_optf_list!=NULL){
                    (*o_optf_list)[optf_len][frame][0] = px;
                    (*o_optf_list)[optf_len][frame][1] = py;
                    (*o_optf_list)[optf_len][frame][2] = pz; 
                }
                //if(o_optf_map!=NULL){
                //    (*o_optf_map)[frame][i_nc] = r;
                //    (*o_optf_map)[frame][i_nc+1] = g;
                //    (*o_optf_map)[frame][i_nc+2] = b;
                //}
                if(in_optf_map[0].IsOutOfBounds(static_cast<int>(px), static_cast<int>(py))==true){ continue; }
                i_d = static_cast<int>(py * w + px);
                i_d_nc = i_d*nc;
                // --------------------------

                //! find optical flows after first frame ----------
                for(frame=1; frame<end; frame++){
                    if(in_optf_map[frame].GetAlpha(i_d)!=255){ break; }
                    //printf("%f %f, ", (*o_optf_list)[optf_len][frame-1][0], (*o_optf_list)[optf_len][frame-1][1]);
                    //printf("%f %f, ", (*o_optf_list)[optf_len][frame][0], (*o_optf_list)[optf_len][frame][1]);
                    //printf("%d %d, ", px, py);

                    optf_x = in_optf_map[frame][i_d_nc];
                    optf_y = in_optf_map[frame][i_d_nc+1];
                    px += optf_x;
                    py += optf_y;
                    //optf_x = in_optf_map[frame][i_d*nc];
                    //optf_y = in_optf_map[frame][i_d*nc+1];
                    if(o_optf_list!=NULL){
                        (*o_optf_list)[optf_len][frame][0] = px;
                        (*o_optf_list)[optf_len][frame][1] = py;
                        (*o_optf_list)[optf_len][frame][2] = (optf_x*optf_x + optf_y*optf_y) * (optf_x*optf_y/abs(optf_x*optf_y));
                    }
                    //if(o_optf_map!=NULL){
                    //    (*o_optf_map)[frame][i_d_nc] = r;
                    //    (*o_optf_map)[frame][i_d_nc+1] = g;
                    //    (*o_optf_map)[frame][i_d_nc+2] = b;
                    //}
                    
                    if(in_optf_map[0].IsOutOfBounds(static_cast<int>(px), static_cast<int>(py))==true){ break; }

                    i_d = static_cast<int>(py * w + px);
                    i_d_nc = i_d*nc;
                }
                // --------------------------
                if(o_optf_list!=NULL){
                    (*o_optf_list)[optf_len].SetLength(frame);
                }
                //printf("frame is %d\r", frame);
                optf_len++;
            }
        }
    }
    if(o_optf_list!=NULL){
        o_optf_list->SetLength(optf_len);
    }

    printf("Opticalflow list successfully extracted, number of opticalflows = %d\n", optf_len);

    return WSP_STATE_SUCCESS;
}

template<typename T>
wsp::State wsp::video::DrawOpticalflow(const wsp::OpticalflowList &in_optf_list, wsp::ImageSeq<T> &io_image)
{
    int nc= io_image.num_of_channels();
    if(nc!=3){
        WSP_COMMON_ERROR_LOG("Number of channels of io_image must be 3\n");
        return WSP_STATE_INVALID_PARAMETER;
    }
    int len = in_optf_list.length();
    int optf_size_threshold = io_image.SeqSize();
    int w = io_image.width();
    int h = io_image.height();

    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        int i, frame, index;
        int optf_size;
        int x, y;
        int r, g, b;

        #ifdef _OPENMP
        #pragma omp for
        #endif
        for(i=0; i<len; ++i){
            optf_size = in_optf_list[i].length();
            //if(optf_size<optf_size_threshold){ continue; }
            r = static_cast<T>(rand()*255 /RAND_MAX);
            g = static_cast<T>(rand()*255 /RAND_MAX);
            b = static_cast<T>(rand()*255 /RAND_MAX);
            for(frame=0; frame<optf_size; frame++){
                x = in_optf_list[i][frame][0];
                y = in_optf_list[i][frame][1];
                index = (y*w+x)*nc;
                io_image[frame][index]  = r;
                io_image[frame][index+1]= g;
                io_image[frame][index+2]= b;
            }
        }
    }

    return WSP_STATE_SUCCESS;
}




#endif