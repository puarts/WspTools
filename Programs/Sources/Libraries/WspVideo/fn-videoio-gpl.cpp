/**
* @file fn-video-gpl.cpp
* Video I/O functions using GPL library like FFmpeg.
* @author Junichi Nishikata
*/

#ifdef __cplusplus
extern "C"{
#endif
    #include <libavutil/avstring.h>
    #include <libavutil/mathematics.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libavformat/avformat.h>

    #include <libavcodec/avcodec.h>
    #include <libavfilter/avfiltergraph.h>
    #include <libavfilter/avfilter.h>
#ifdef __cplusplus
}
#endif

#include <wsp/image/tpl_cl-image.h>

#include <wsp/video/fn-videoio-gpl.h>
#include <wsp/math/tpl_fn-basic_math.h>

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename)
{
    FILE *f;
    int i;

    f=fopen(filename,"w");
    fprintf(f,"P5\n%d %d\n%d\n",xsize,ysize,255);
    for(i=0;i<ysize;i++)
        fwrite(buf + i * wrap,1,xsize,f);
    fclose(f);
}



wsp::State wsp::video::avLoadVideoAsU8(
    uchar **o_rgba, 
    int &o_width, 
    int &o_height, 
    int &o_num_channels, 
    const char *filename
){
    // initialize libavcode
    //avcodec_register_all();
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
    if(state < 0){
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return WSP_STATE_FAILURE;
    }

    state = av_find_stream_info(fmt_ctx);
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
    //state = avcodec_open(dec_ctx, dec);
    state = avcodec_open(dec_ctx, dec);
    if(state < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
        return WSP_STATE_FAILURE;
    }

    o_width = dec_ctx->coded_width;
    o_height = dec_ctx->coded_height;
    o_num_channels = 3; //frame.linesize[0]/o_width;//3;//dec_ctx->channels;
    int wstep = o_width*o_num_channels;
    int num_pix = o_width*o_height;

    //* read all packets */
    int frame_num = 0;
    wsp::AllocArrayMem(o_rgba, num_pix*o_num_channels);
    while(true){
        state = av_read_frame(fmt_ctx, &packet);
        if(state < 0){
            break;
        }
        if (packet.stream_index == video_stream_index) {
            avcodec_get_frame_defaults(&frame);
            got_frame = 0;
            state = avcodec_decode_video2(dec_ctx, &frame, &got_frame, &packet);
            av_free_packet(&packet);
            if(state < 0){
                av_log(NULL, AV_LOG_ERROR, "Error decoding video\n");
                break;
            }

            if(got_frame){
                if (frame.pts == AV_NOPTS_VALUE){
                    frame.pts = frame.pkt_dts == AV_NOPTS_VALUE ? frame.pkt_dts : frame.pkt_pts;
                }
               
                int x, y, i;
                for(y=0;y<o_height;++y){
                    for(x=0;x<o_width;++x){
                        // Yellow
                        int intensity = static_cast<int>(frame.data[0][y * frame.linesize[0] + x]);
                        // Blue
                        int cb = static_cast<int>(frame.data[1][(y/2) * frame.linesize[1] + x/2]);
                        // red
                        int cr = static_cast<int>(frame.data[2][(y/2) * frame.linesize[2] + x/2]);

                        int r, g, b;
                        wsp::img::ConvertYCbCrToRGB(intensity, cb, cr, r, g, b);
                        wsp::math::Limit(r, 0, 255);
                        wsp::math::Limit(g, 0, 255);
                        wsp::math::Limit(b, 0, 255);

                        (*o_rgba)[y*wstep+x*o_num_channels+0] = uchar(r);
                        (*o_rgba)[y*wstep+x*o_num_channels+2] = uchar(b);
                        (*o_rgba)[y*wstep+x*o_num_channels+1] = uchar(g);
                    }
                }

            
                //pgm_save(frame.data[0], frame.linesize[0], o_width, o_height,     "av_test_0.pgm");
                //pgm_save(frame.data[1], frame.linesize[1], o_width/2, o_height/2, "av_test_1.pgm");
                //pgm_save(frame.data[2], frame.linesize[2], o_width/2, o_height/2, "av_test_2.pgm");

                wsp::Image<uchar> img(*o_rgba, NULL, NULL, o_width, o_height, o_num_channels);
                char dst_name[128];
                sprintf(dst_name, "av_test.%4d.png", frame_num);
                img.SaveImage(dst_name);
                ++frame_num;
            }
        }
    }
    printf("successfully loaded\n");

    return WSP_STATE_SUCCESS;
}


