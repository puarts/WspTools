/**
 * @file tpl_cl-sound.h
 * 
 */

#ifndef _WSP_SOUND_TEMPLATE_CLASS_SOUND_H_
#define _WSP_SOUND_TEMPLATE_CLASS_SOUND_H_

#include <wsp/sound/_define_sound.h>
#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
namespace wsp{
    template<typename T>
    class Sound
        :public wsp::Vector<T>
    {
    public:

        /* I/O */
        wsp::Result Save( const char *filepath )
        {
            WSP_COMMON_WARNING_LOG("Saving sound type is unsupported\n");
            return ::WSP_ResultStatusError();
        }
    };
}

#endif
