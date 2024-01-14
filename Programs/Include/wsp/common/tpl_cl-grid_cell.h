// @file tpl_cl-grid_cell.h
// 


#ifndef _WSP_CORE_TEMPLATE_CLASS_GRID_CELL_H_
#define _WSP_CORE_TEMPLATE_CLASS_GRID_CELL_H_

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include "tpl_cl-vector.h"
#endif
#include "tpl_cl-vector3.h"

namespace wsp{
    //template<typename ELEM_TYPE>
    //class GridCell: public wsp::Vector<ELEM_TYPE>
    //{
    ////private:
    ////    wsp::Vector<ELEM_TYPE> assigned_elements_;
    ////public:
    ////    // Getters
    ////    inline wsp::Vector<ELEM_TYPE> assigned_elements(){ return assigned_elements_; }

    ////    // Mutators
    ////    inline void AppendElement(ELEM_TYPE elem){ assigned_particles_.Append(elem); }
    ////    inline void Clear(){ assigned_elements_.Clear(); }
    //};


    //! \ingroup WspCommonUtil
    //! \brief Grid cell list manipulation 
    /*!
      wsp::GridCellList is a template class which provides access to grid cell list
      manipulation functionality. 
    */
    template<typename ELEM_TYPE>
    class GridCellList: public wsp::Vector<wsp::Vector<ELEM_TYPE>>
    {
    private:
        wsp::Vector3<u32> num_of_cell_;
        wsp::Vector3<double> cell_size_;

    public:
        // Constructors
        inline GridCellList(const wsp::Vector3<u32> &num_of_cell, const wsp::Vector3<double> &cell_size){
            set_num_of_cell(num_of_cell);
            set_cell_size(cell_size);
        }
        // Getters
        inline wsp::Vector3<u32> num_of_cell() const{ return num_of_cell_; }
        inline wsp::Vector3<double> cell_size() const{ return cell_size_; }

        // Mutators
        inline void set_num_of_cell(const wsp::Vector3<u32> &num_of_cell){ 
            wsp::Vector<wsp::Vector<ELEM_TYPE>>::SetLength(num_of_cell[0] * num_of_cell[1] * num_of_cell[2]);
            num_of_cell_ = num_of_cell; 
        }
        inline void set_cell_size(const wsp::Vector3<double> &cell_size){ cell_size_ = cell_size; }
        inline void Clear(){
            for(u32 i=0; i<wsp::Vector<wsp::Vector<ELEM_TYPE>>::length_; i++){
                wsp::Vector<wsp::Vector<ELEM_TYPE>>::data_[i].Clear();
            }
        }
    };
}

#endif