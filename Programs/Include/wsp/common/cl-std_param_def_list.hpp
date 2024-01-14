/**
 * @file cl-std_param_def_list.hpp
 * 
 */

#ifndef WSP_CORE_CLASS_WSP_STD_PARAM_DEF_LIST_HPP__
#define WSP_CORE_CLASS_WSP_STD_PARAM_DEF_LIST_HPP__

#include "_cl-std_param.h"
#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include "tpl_cl-vector.h"
#endif

namespace wsp{
    class StdParamDefList
        : public wsp::Vector<wsp::StdParamDefinition>
    {
    public:
        StdParamDefList()
            : use_auto_param_estimation_(false)
        {}

        /* Getter */
        inline bool use_auto_param_estimation()const{ return use_auto_param_estimation_; }

        /* Setter */
        inline void set_use_auto_param_estimation( bool use_auto_param_estimation ){ use_auto_param_estimation_ = use_auto_param_estimation; }

    protected:
        bool use_auto_param_estimation_;

    };
}

#endif
