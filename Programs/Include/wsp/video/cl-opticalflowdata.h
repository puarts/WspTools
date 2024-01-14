/**
 * @file cl-opticalflowdata.h
 * @author Junichi Nishikata
 * Opticalflow handling class.
 */


#ifndef _WSP_OPTICALFLOW_DATA_H_
#define _WSP_OPTICALFLOW_DATA_H_

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/common/tpl_cl-vector3.h>

namespace wsp{

    const int kOpticalflowDataMemStep = 50; //! optimize for frame length
    const int kOpticalflowListMemStep = 100; //! optimize for number of pixels

    class OpticalflowData
        : public wsp::Vector<wsp::Point<double>, kOpticalflowDataMemStep>
    {
    public: 
        inline OpticalflowData()
            : wsp::Vector<wsp::Point<double>, wsp::kOpticalflowDataMemStep>()
        {}
        inline OpticalflowData(const wsp::OpticalflowData &data)
            : wsp::Vector<wsp::Point<double>, wsp::kOpticalflowDataMemStep>(data)
        {}
    };


    class OpticalflowList
        : public wsp::Vector<wsp::OpticalflowData, wsp::kOpticalflowListMemStep>
    {
    public: 
        inline OpticalflowList()
            : wsp::Vector<wsp::OpticalflowData, wsp::kOpticalflowListMemStep>()
        {}
        inline OpticalflowList(const wsp::OpticalflowList &data)
            : wsp::Vector<wsp::OpticalflowData, wsp::kOpticalflowListMemStep>(data)
        {}
    };
}
#endif