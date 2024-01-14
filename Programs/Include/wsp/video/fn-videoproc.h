//! fn-videoproc.h
// @author Jun-ichi Nishikata
//! video processing library

#ifndef WspVideo_WspFnVideoProc_H_
#define WspVideo_WspFnVideoProc_H_

#include "_define_video.h"
#include <wsp/image/tpl_cl-imageseq.h>

namespace wsp{
    class OpticalflowData;
    class OpticalflowList;
}

//namespace wsp{ namespace img{
//    struct StereoBMParams;
//}}


namespace wsp{ namespace video{
    //! Optical flow -----------------------------------------------
    WSP_DLL_EXPORT wsp::State RemoveFixedOpticalflow(wsp::OpticalflowList &io_optf_list, int threshold_fixed_frame);

    ////! calculate opticalflow from sequence by block matching algorithm
    //WSP_DLL_EXPORT wsp::State GetOpticalFlowBM(wsp::ImageSeq<double> *o_dst, 
    //                            wsp::ImageSeq<uchar> *o_dst_norm_seq, 
    //                            const wsp::ImageSeq<uchar> &in_src, 
    //                            const wsp::img::StereoBMParams &params,
    //                            int frame_offset=1,
    //                            wsp::video::OpticalflowDisplayType out_disp_type=wsp::video::OPTICALFLOW_DISPLAY_SCALAR_XY,
    //                            bool save_norm_data=true,
    //                            bool save_raw_data=true
    //                            #ifdef USE_QT
    //                            , QLabel *image_label=NULL, QProgressBar *progress_bar=NULL
    //                            #endif
    //                            );

    //inline wsp::State GetOpticalFlowBM(wsp::ImageSeq<short int> *o_dst,    wsp::ImageSeq<uchar> *o_dst_norm_seq, 
    //                            const wsp::ImageSeq<uchar> &in_src, const wsp::img::StereoBMParams &params, int frame_offset=1,
    //                            wsp::video::OpticalflowDisplayType out_disp_type=wsp::video::OPTICALFLOW_DISPLAY_SCALAR_XY,
    //                            bool save_norm_data=true, bool save_raw_data=true
    //                            #ifdef USE_QT
    //                            , QLabel *image_label=NULL, QProgressBar *progress_bar=NULL
    //                            #endif
    //                            )
    //{
    //    wsp::State state;
    //    wsp::ImageSeq<double> optf_seq;
    //    state = wsp::video::GetOpticalFlowBM(&optf_seq, o_dst_norm_seq, in_src, params, frame_offset, out_disp_type, save_norm_data, save_raw_data
    //                            #ifdef USE_QT
    //                            , image_label, progress_bar
    //                            #endif
    //                            );
    //    if(state!=WSP_STATE_SUCCESS){ return state; }
    //    if(o_dst!=NULL){ o_dst->CopyFrom(optf_seq); }
    //    return state;
    //}

    ////! extract only continuous opticalflow from opticalflow map sequence
    ////! end_frame=-1 means end_frame is to find opticalflow to last frame in the sequecne
    //WSP_DLL_EXPORT wsp::State ExtractContinuousOpticalflowList(wsp::OpticalflowList &o_optf_list, 
    //                            const wsp::ImageSeq<double> &in_optf_map, int start_frame=0, int end_frame=-1, int pix_step=1);
}}

#endif