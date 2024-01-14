/**
 * @file _define_graphic.h
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_GUI_DEFINE_GUI_H_
#define WSP_GUI_DEFINE_GUI_H_

#ifdef _WINDLL
    #define USE_QT
#endif

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif
#include <wsp/math/_define_mathutil.h>
#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include <wsp/image/_define_image.h>
#endif
#include <wsp/graphics/_define_graphic.h>


#ifdef USE_QT
#include <wsp/common/_define_qt.h>
#endif



namespace wsp{
    /**
     * @brief Collection of functions and classes for WspGui module
     */
    namespace gui{

        typedef enum WindowType{
            WINDOW_TYPE_IMAGE_VIEW,
            WINDOW_TYPE_COMP_VIEW,
            WINDOW_TYPE_LAYER_VIEW,
            WINDOW_TYPE_GENERAL_VIEW,
            WINDOW_TYPE_UNKNOWN
        } WindowType;

        inline const char* GetWindowTypeAsString(wsp::gui::WindowType win_type)
        {
            switch(win_type)
            {
            case wsp::gui::WINDOW_TYPE_IMAGE_VIEW:   return "WINDOW_TYPE_IMAGE_VIEW";
            case wsp::gui::WINDOW_TYPE_COMP_VIEW:    return "WINDOW_TYPE_COMP_VIEW";
            case wsp::gui::WINDOW_TYPE_LAYER_VIEW:   return "WINDOW_TYPE_LAYER_VIEW";
            case wsp::gui::WINDOW_TYPE_GENERAL_VIEW: return "WINDOW_TYPE_GENERAL_VIEW";
            }
            return "WINDOW_TYPE_UNKNOWN";
        }
    }
}




#endif