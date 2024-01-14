/**
 * @file _cl-noncopyable.h
 * 
 */

#ifndef __WSP_CORE_CLASS_NONCOPYABLE_H__
#define __WSP_CORE_CLASS_NONCOPYABLE_H__

#define WSP_NONCOPYABLE(class_name)\
    private:\
        class_name(const class_name &);\
        const class_name& operator=(const class_name &)

namespace wsp{
    class NonCopyable
    {
        WSP_NONCOPYABLE(NonCopyable);
    public:
        inline NonCopyable(){}
        inline ~NonCopyable(){}
    };
}

#endif
