/**
 * @file tpl_fn-video-gpl.h
 * @author Junichi Nishikata
 * Video I/O template functions using GPL library like FFmpeg.
 */

#ifndef _WSP_VIDEO_TEMPLATE_FN_VIDEOIO_GPL_H_
#define _WSP_VIDEO_TEMPLATE_FN_VIDEOIO_GPL_H_

#ifdef __cplusplus
extern "C"{
#endif
    #ifdef USE_LIBAV
        #include <libavutil/imgutils.h>
        #include <libavutil/avstring.h>
        #include <libavutil/mathematics.h>
        #include <libavcodec/avcodec.h>
        #include <libswscale/swscale.h>
        #include <libavformat/avformat.h>

        #include <libavcodec/avcodec.h>
        #include <libavfilter/avfiltergraph.h>
        //#include <libavfilter/buffersrc.h>
    #endif
#ifdef __cplusplus
}
#endif

#include <wsp/image/tpl_cl-imageseq.h>
#include "_define_video.h"


namespace wsp{ namespace video{
    #ifdef USE_LIBAV
    template<typename T>
    wsp::State avLoadVideo(
        wsp::ImageSeq<T> &o_imgseq, 
        const char *filename
    );
    #endif
}}

#ifdef USE_LIBAV
template<typename T>
wsp::State wsp::video::avLoadVideo(wsp::ImageSeq<T> &o_imgseq, const char *filename){
    av_register_all();
    
    int i;
    int state;
    AVCodec *dec;
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *dec_ctx = NULL;
    AVPacket packet;
    AVFrame frame;
    int got_frame;
    
    state = avformat_open_input(&fmt_ctx, filename, NULL, NULL);
    //state = avcodec_open2(c, NULL, NULL);
    if(state < 0){
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return WSP_STATE_FAILURE;
    }

    state = avformat_find_stream_info(fmt_ctx, NULL);
    if (state < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return WSP_STATE_FAILURE;
    }

    //* select the video stream */
    int video_stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
    if (video_stream_index < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
        return WSP_STATE_FAILURE;
    }
    dec_ctx = fmt_ctx->streams[video_stream_index]->codec;

    //* init the video decoder */
    state = avcodec_open(dec_ctx, dec);
    if(state < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
        return WSP_STATE_FAILURE;
    }

    int width = dec_ctx->coded_width;
    int height = dec_ctx->coded_height;
    int num_channels = 3;//frame.linesize[0]/o_width;//3;//dec_ctx->channels;
    int wstep = width*num_channels;
    int num_pix = width*height;
    int num_frame = fmt_ctx->streams[video_stream_index]->nb_frames;

    
    o_imgseq.SetSeqSize(num_frame);

    printf("num_frame %d\n", num_frame);

    //* read all packets */
    int frame_num = 0;
    //while(true){
    int decoded_data_byte = 0;
    int decoded_frame = 0;
    int audio_frame = 0;
    const int genpts= fmt_ctx->flags & AVFMT_FLAG_GENPTS;
    do{
        audio_frame = 0;
        state = av_read_frame(fmt_ctx, &packet);
        if(packet.stream_index == video_stream_index){
            avcodec_get_frame_defaults(&frame);
            got_frame = 0;
            decoded_data_byte = avcodec_decode_video2(dec_ctx, &frame, &got_frame, &packet);
            if(decoded_data_byte>0){ ++decoded_frame; }
            if(got_frame==0 && frame_num!=0){ break; }

            av_free_packet(&packet);

            if(got_frame){
                printf("got frame %d\n", frame_num);
                if (frame.pts == AV_NOPTS_VALUE){
                    frame.pts = frame.pkt_dts == AV_NOPTS_VALUE ? frame.pkt_dts : frame.pkt_pts;
                }

                o_imgseq[frame_num].SetSize(width, height, num_channels);

                //int linesizes[4];
                //av_image_fill_linesizes(linesizes, dec_ctx->pix_fmt, width);

                int x, y, i;
                wsp::Image<T> *cur_img = &o_imgseq[frame_num];

                //int buf_len = avpicture_get_size(dec_ctx->pix_fmt, width, height);
                //int add_val = (-frame.linesize[0]+4-1);
                //int and_val = (~(4-1));
                //int linesize_test = add_val&and_val;



                // convert into wsp::Image
                switch(dec_ctx->pix_fmt){
                case PIX_FMT_BGR24:
                    {
                        int linesize = -frame.linesize[0];
                        u328_t *src_ptr = frame.data[0];
                        for(y=0;y<height;++y){
                            for(x=0;x<width;++x, src_ptr+=3){
                                i = y*width + x;
                                (*cur_img)[y*wstep+x*num_channels]  = static_cast<T>(*(src_ptr+2));
                                (*cur_img)[y*wstep+x*num_channels+1]= static_cast<T>(*(src_ptr+1));
                                (*cur_img)[y*wstep+x*num_channels+2]= static_cast<T>(*src_ptr);
                            }
                        }
                    }
                    break;
                case PIX_FMT_YUV420P:
                    for(y=0;y<height;++y){
                        for(x=0;x<width;++x){
                            // Y
                            int intensity = static_cast<int>(frame.data[0][y * frame.linesize[0] + x]);
                            // Cb
                            int cb = static_cast<int>(frame.data[1][(y/2) * frame.linesize[1] + x/2]);
                            // Cr
                            int cr = static_cast<int>(frame.data[2][(y/2) * frame.linesize[2] + x/2]);

                            int r, g, b;
                            wsp::img::ConvertYCbCrToRGB(intensity, cb, cr, r, g, b);
                            wsp::math::Limit(r, 0, 255);
                            wsp::math::Limit(g, 0, 255);
                            wsp::math::Limit(b, 0, 255);

                            (*cur_img)[y*wstep+x*num_channels] = static_cast<T>(r);
                            (*cur_img)[y*wstep+x*num_channels+1] = static_cast<T>(g);
                            (*cur_img)[y*wstep+x*num_channels+2] = static_cast<T>(b);
                        }
                    }
                    break;
                default:
                    WSP_COMMON_ERROR_LOG("undefined wsp::Image conversion\n");
                }
                //pgm_save(frame.data[0], frame.linesize[0], o_width, o_height,     "av_test_0.pgm");
                //pgm_save(frame.data[1], frame.linesize[1], o_width/2, o_height/2, "av_test_1.pgm");
                //pgm_save(frame.data[2], frame.linesize[2], o_width/2, o_height/2, "av_test_2.pgm");

                //wsp::Image<uchar> img(*o_rgba, NULL, NULL, o_width, o_height, o_num_channels);
                //char dst_name[128];
                //sprintf(dst_name, "av_test.%4d.png", frame_num);
                //img.SaveImage(dst_name);
                ++frame_num;
            }else{
                printf("faild to get frame %d\n", frame_num);
            }
        }else{
            printf("packet.stream_index not equals to video_stream_index\n");
            ++audio_frame;
        }
    }while((frame_num<decoded_frame) || (audio_frame==1));

    return WSP_STATE_SUCCESS;
}
#endif

#endif

