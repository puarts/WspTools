/**
 * @file tpl_cl-function_lane.hpp
 * 
 */

#ifndef WSP_CORE_TEMPLATE_CLASS_FUNCTION_LANE_HPP_
#define WSP_CORE_TEMPLATE_CLASS_FUNCTION_LANE_HPP_

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif

#define MAX_NUM_OF_FUNC_LANE 20

namespace wsp{

    template <typename _LaneClass>
    class FunctionLane
    {
    protected:
        typedef void (_LaneClass::*LaneFuncPtr)(); 

    protected:
        _LaneClass* func_handle_ptr_;
        LaneFuncPtr func_ptr_list_[MAX_NUM_OF_FUNC_LANE]; 
        int num_of_funcs_; 

    public:
        FunctionLane(_LaneClass& rHandlee)
            : func_handle_ptr_(&rHandlee)
            , num_of_funcs_(0)
        {}

        void Set(_LaneClass& handle) 
        { 
            func_handle_ptr_ = &handle; 
        }

    public:
        bool Push(LaneFuncPtr lane_func_ptr);
        void Run();
        void Clear(){ num_of_funcs_ = 0; }
    };
}

template <typename _LaneClass>
bool wsp::FunctionLane"_LaneClass>::Push(LaneFuncPtr lane_func_ptr)
{
    if(num_of_funcs_ >= MAX_NUM_OF_FUNC_LANE){
        WSP_COMMON_ERROR_LOG("Too many fucntions registered\n");
        return false;
    }
    func_ptr_list_[num_of_funcs_++] = lane_func_ptr;
    return true;
}

template <typename _LaneClass>
void wsp::FunctionLane"_LaneClass>::Run()
{
    for(int i = 0; i < num_of_funcs_; i++){
        (func_handle_ptr_->*func_ptr_list_[i])();
    }
}


#endif
