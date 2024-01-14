#pragma once

#include <wsp/common/_define_commoncore.h>
#include <wsp/common/common_types.h>

namespace wsp{ namespace opencv{
    // ==============================
    //! Segmentation 
    // ==============================

    //WSP_DLL_EXPORT wsp::State cvSegmPyrMeanShiftFiltering(uchar *o_segm, uchar *o_colorSegm, const uchar *in_img, u32 width, u32 height, u32 nChannels,
    //                                                        int pyrLevel=2, double winRad_space=30.0, double winRad_colSpace=30.0);
    WSP_DLL_EXPORT 
    wsp::State cvSegmPyrMeanShiftFiltering(
        int *o_segm, 
        const uchar *in_img, 
        u32 width, u32 height, u32 nChannels, 
        double winRad_space=30.0, 
        double winRad_colSpace=30.0, 
        int pyrLevel=2
    );

    //WSP_DLL_EXPORT wsp::State cvSegmPyr(int *o_segm, uchar *o_colorSegm, const uchar *in_img, u32 width, u32 height, u32 nChannels,
    //                                        int pyrLevel=4, double thre_link=255.0, double thre_segmClast=50.0);
    WSP_DLL_EXPORT 
    wsp::State cvSegmPyr(
        int *o_segm, 
        const uchar *in_img, 
        u32 width, u32 height, u32 nChannels,
        int pyrLevel=4, 
        double thre_link=255.0, 
        double thre_segmClast=50.0
    );

    WSP_DLL_EXPORT 
    wsp::State cvSegmWatershedPaintMarkers(
        int *o_segm, 
        const uchar *in_img, 
        u32 width, u32 height, u32 nChannels
    );

    WSP_DLL_EXPORT 
    wsp::State cvSegmWatershed(
        int *io_segm, 
        const uchar *in_img, 
        u32 width, u32 height, u32 nChannels
    );

}}

