//! WspFnVideoProc.cpp
//! Jun-ichi Nishikata

#include <stdio.h>
#include <wsp/image/fn-imgproc.h>
#include <wsp/image/gpl_tpl_fn-imgproc.h>
#include <wsp/video/fn-videoproc.h>
#include <wsp/video/cl-opticalflowdata.h>

#ifdef USE_OPENMAYA
#include <wsp/maya/_define_maya.h>
#endif


//! =========================================================================================
//! =========================================================================================
//!
//!                                  Optical flow 
//!
//! =========================================================================================
//! =========================================================================================

wsp::State wsp::video::RemoveFixedOpticalflow(wsp::OpticalflowList &io_optf_list, int threshold_fixed_frame)
{
    int len = io_optf_list.length();

    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        int i, frame;
        int optf_size;
        int x, y;
        int cnt;

        #ifdef _OPENMP
        #pragma omp for
        #endif
        for(i=0; i<len; ++i){
            optf_size = io_optf_list[i].length();
            if(optf_size==0){ continue; }
            x = io_optf_list[i][0][0];
            y = io_optf_list[i][0][1];
            cnt=0;
            for(frame=1; frame<optf_size; frame++){
                if(x == io_optf_list[i][frame][0] && y == io_optf_list[i][frame][1]){
                    cnt++;
                }else{
                    cnt=0;
                }
                if(cnt>=threshold_fixed_frame){
                    io_optf_list[i].SetLength(0);
                    break;
                }
                x = io_optf_list[i][frame][0];
                y = io_optf_list[i][frame][1];
            }
        }
    }
    return WSP_STATE_SUCCESS;
}


//wsp::State wsp::video::GetOpticalFlowBM(wsp::ImageSeq<double> *o_dst_raw_seq,
//                                        wsp::ImageSeq<uchar> *o_dst_norm_seq, 
//                                        const wsp::ImageSeq<uchar> &in_src_seq, 
//                                        const wsp::img::StereoBMParams &params,
//                                        int frame_offset, 
//                                        wsp::video::OpticalflowDisplayType out_disp_type,
//                                        bool save_norm_data,
//                                        bool save_raw_data
//#ifdef USE_QT
//                                        , QLabel *image_label
//                                        , QProgressBar *progress_bar
//#endif
//                                        )
//{
//    //wsp::Image<uchar> img_left, img_right;
//    wsp::Image<uchar> img_left,img_right;
//    wsp::Image<double> img_disp;
//    wsp::State state;
//    wsp::SeqInfo info;
//    char filename[MAX_LENGTH_OF_PATH];
//    char filename_alpha[MAX_LENGTH_OF_PATH];
//
//    int start_frame = in_src_seq.SeqInfoSrc().start_frame;
//
//    int seq_size = static_cast<int>(in_src_seq.SeqSize()) - frame_offset;
//    if(o_dst_raw_seq!=NULL){  o_dst_raw_seq->SetSeqSize(seq_size); }
//    if(o_dst_norm_seq!=NULL){ o_dst_norm_seq->SetSeqSize(seq_size); }
//    int end_frame = start_frame + seq_size;
//
//    #ifdef USE_QT
//    if(progress_bar!=NULL){
//        progress_bar->setRange(0, seq_size); progress_bar->setValue(0);
//    }
//    #endif
//    for(int i=0, frame=start_frame; i<seq_size; i++, frame++)
//    {
//        printf("\nprocessing %d frame ----------------------------------\n", frame);
//        #ifdef USE_QT
//        if(progress_bar!=NULL){ progress_bar->setValue(i); }
//        #endif
//
//        in_src_seq[i].CopyIntoAnyImage(img_left);
//        in_src_seq[i+frame_offset].CopyIntoAnyImage(img_right);
//
//        state=wsp::img::StereoBM(img_disp, img_left, img_right, params);
//        if(state!=WSP_STATE_SUCCESS){ 
//            #ifdef USE_QT
//            if(progress_bar!=NULL){ progress_bar->setValue(0); }
//            #endif
//            WSP_COMMON_ERROR_LOG("Stereo matching failed\n"); return state; 
//        }
//        img_disp.ResetAlpha(1);
//        img_disp.SetAlphaAtTargetStatePix(0, WSP_PIXEL_STATE_OCCLUDED);
//
//        //! saving raw data
//        if(save_raw_data){
//            //sprintf(filename, "disp_raw.%04d.wspid", frame);
//            //img_disp.SaveRawData(filename);
//            sprintf(filename, "%s.%04d.adri", in_src_seq.SeqInfoSrc().name, frame);
//            sprintf(filename_alpha, "%s_alpha.%04d.adri", in_src_seq.SeqInfoSrc().name, frame);
//            img_disp.SaveAsADRI(filename, filename_alpha);
//        }
//        img_disp.NormalizeAlpha(0, 255);
//        if(o_dst_raw_seq!=NULL){
//            (*o_dst_raw_seq)[i] = img_disp;
//        }
//        switch(out_disp_type){
//        case wsp::video::OPTICALFLOW_DISPLAY_SCALAR_XY: 
//            img_disp.Normalize(0, 255);
//            //img_disp.CombineChannelsByNorm(); 
//            img_disp.CombineChannelsBySum(); 
//            break;
//        case wsp::video::OPTICALFLOW_DISPLAY_X: img_disp.LeaveOneChannel(0); break;
//        case wsp::video::OPTICALFLOW_DISPLAY_Y: img_disp.LeaveOneChannel(1); break;
//        default: break;
//        }
//
//        img_disp.Normalize(0, 255);
//        //printf("///////////// num of channel:%d\n", img_disp.num_of_channels());
//        //info = in_src_seq.SeqInfoSrc();
//        //sprintf(filename, "%s/%s_disp.%4d.%s", info.dir, info.name, frame, info.ext);
//        if(o_dst_norm_seq!=NULL){
//            //(*o_dst_norm_seq)[frame].CopyFrom(img_disp, o_dst_norm_seq->num_of_channels());
//            img_disp.CopyIntoAnyImage((*o_dst_norm_seq)[i], in_src_seq.num_of_channels());
//        }
//        if(save_norm_data){
//            sprintf(filename, "%s.%04d.png", in_src_seq.SeqInfoSrc().name, frame);
//            img_disp.SaveImage(filename);
//        }
//        #ifdef USE_QT
//        if(image_label!=NULL){
//            img_disp.SetAlphaAtTargetStatePix(0, WSP_PIXEL_STATE_OCCLUDED);
//            //img_disp.CopyIntoAnyImage(in_src_seq[frame], in_src_seq.num_of_channels());
//            wsp::img::qtWspImageToQLabel(*image_label, in_src_seq[i]);
//        }
//        #endif
//    }
//    #ifdef USE_QT
//    if(progress_bar!=NULL){ progress_bar->setValue(0); }
//    #endif
//    return WSP_STATE_SUCCESS;
//}
//
//wsp::State wsp::video::ExtractContinuousOpticalflowList(wsp::OpticalflowList &o_optf_list, 
//                        const wsp::ImageSeq<double> &in_optf_map, int start_frame, int end_frame, int pix_step)
//{
//    if(start_frame>end_frame && end_frame!=-1){
//        WSP_COMMON_ERROR_LOG("Invalid start_frame or end_frame\n"); return WSP_STATE_INVALID_PARAMETER;
//    }
//    int w = static_cast<int>(in_optf_map.width());
//    int h = static_cast<int>(in_optf_map.height());
//    int nc= static_cast<int>(in_optf_map.num_of_channels());
//    int len = w*h;
//    int seq_size = static_cast<int>(in_optf_map.SeqSize());
//    int end = end_frame < 1? seq_size : end_frame; 
//    if(seq_size<2 || len==0 || nc==0){
//        WSP_COMMON_ERROR_LOG("Opticalflow map size is not enough\n"); return WSP_STATE_INVALID_PARAMETER;
//    }
//
//    printf("Opticalflow source data info: \n");
//    printf("w=%d, h=%d, nc=%d, seq_size=%d\n", w, h, nc, seq_size);
//
//    o_optf_list.SetLength(len);
//
//    int optf_len=0;
//    int cnt = 0;
//
//    #ifdef _OPENMP
//    #pragma omp parallel
//    #endif
//    {
//        double px, py;
//        double optf_x, optf_y;
//        int x, y, i, i_d, frame;
//
//
//        #ifdef _OPENMP
//        #pragma omp for reduction(+:optf_len, cnt)
//        #endif
//        for(y=0; y<h; y+=pix_step){
//            for(x=0; x<w; x+=pix_step){
//                i = y*w + x;
//                #ifdef USE_OPENMAYA
//                wsp::maya::Printf("(y, x)=(%d, %d)\n", y, x);
//                #else
//                //printf("(y, x)=(%d, %d), ", y, x);
//                cnt++;
//                printf("thread %d: %5f %% done..\r", omp_get_thread_num(), cnt*100*omp_get_num_threads()/static_cast<float>(len));
//                #endif
//                uchar alpha = in_optf_map[start_frame].GetAlpha(i);
//                if(alpha<0xff){ continue; }
//                //printf("alpha = %d\n", alpha);
//
//                o_optf_list[optf_len].SetLength(seq_size);
//                frame = start_frame;
//                //! add first optical flow ------------------------
//                optf_x = in_optf_map[frame][i*nc];
//                optf_y = in_optf_map[frame][i*nc+1];
//                o_optf_list[optf_len][frame][0] = x + optf_x;
//                o_optf_list[optf_len][frame][1] = y + optf_y;
//                //! define z as magnitude of optical flow instead of actual depth
//                o_optf_list[optf_len][frame][2] = (optf_x*optf_x + optf_y*optf_y) * (optf_x*optf_y/abs(optf_x*optf_y)); 
//                px = o_optf_list[optf_len][frame][0];
//                py = o_optf_list[optf_len][frame][1];
//                if(in_optf_map[0].IsOutOfBounds(static_cast<int>(px), static_cast<int>(py))==true){ continue; }
//                i_d = static_cast<int>(o_optf_list[optf_len][frame][1]) * w + static_cast<int>(o_optf_list[optf_len][frame][0]);
//                // --------------------------
//
//                //! find optical flows after first frame ----------
//                for(frame=1; frame<end; frame++){
//                    if(in_optf_map[frame].GetAlpha(i_d)!=255){ break; }
//                    //printf("%f %f, ", o_optf_list[optf_len][frame-1][0], o_optf_list[optf_len][frame-1][1]);
//                    //printf("%f %f, ", o_optf_list[optf_len][frame][0], o_optf_list[optf_len][frame][1]);
//                    //printf("%d %d, ", px, py);
//
//                    optf_x = in_optf_map[frame][i_d*nc];
//                    optf_y = in_optf_map[frame][i_d*nc+1];
//                    px += optf_x;
//                    py += optf_y;
//                    //optf_x = in_optf_map[frame][i_d*nc];
//                    //optf_y = in_optf_map[frame][i_d*nc+1];
//                    o_optf_list[optf_len][frame][0] = px;
//                    o_optf_list[optf_len][frame][1] = py;
//                    o_optf_list[optf_len][frame][2] = (optf_x*optf_x + optf_y*optf_y) * (optf_x*optf_y/abs(optf_x*optf_y));
//                    if(in_optf_map[0].IsOutOfBounds(static_cast<int>(px), static_cast<int>(py))==true){ break; }
//
//                    i_d = static_cast<int>(py * w + px);
//                }
//                // --------------------------
//                o_optf_list[optf_len].SetLength(frame);
//                //printf("frame is %d\r", frame);
//                optf_len++;
//            }
//        }
//    }
//    o_optf_list.SetLength(optf_len);
//
//    printf("Opticalflow list successfully extracted, number of opticalflows = %d\n", optf_len);
//
//    return WSP_STATE_SUCCESS;
//}
