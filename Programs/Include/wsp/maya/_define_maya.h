/**
 * @file _define_maya.h
 * @author Jun-ichi Nishikata
 * Base definition of core module for OpenMaya.
 */

#ifndef WSP_CORE_DEFINE_MAYA_H__
#define WSP_CORE_DEFINE_MAYA_H__

#ifdef _WINDLL
    #ifndef USE_OPENMAYA
        #define USE_OPENMAYA
    #endif
#endif

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#ifdef USE_OPENMAYA

#ifndef _BOOL
#define _BOOL
#endif
#ifndef REQUIRE_IOSTREAM
#define REQUIRE_IOSTREAM
#endif
#ifndef NT_PLUGIN
#define NT_PLUGIN
#endif

#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnLightDataAttribute.h> 


namespace wsp{ namespace maya{
    inline void Printf(const char *fmt, ...){
        char str[MAX_BUFFER];
        va_list ap;
        va_start(ap, fmt);
        vsprintf(str, fmt, ap);
        MGlobal::displayInfo(::MString(&str[0]));
        va_end(ap);
    }
    inline void Error(char *file, int line, const char *fmt, ...){
        char str[MAX_BUFFER], header_str[MAX_BUFFER];
        va_list ap;
        va_start(ap, fmt);
        sprintf(header_str, "%s %d : ", file, line);
        vsprintf(str, fmt, ap);
        MGlobal::displayError(MString(&header_str[0])+MString(&str[0]));
        va_end(ap);
    }
    inline void Warning(char *file, int line, const char *fmt, ...){
        char str[MAX_BUFFER], header_str[MAX_BUFFER];
        va_list ap;
        va_start(ap, fmt);
        sprintf(header_str, "%s %d : ", file, line);
        vsprintf(str, fmt, ap);
        MGlobal::displayWarning(MString(&header_str[0])+MString(&str[0]));
        va_end(ap);
    }
    inline void DebugPrintf(const char *fmt, ...){
        #ifdef _DEBUG
        char str[MAX_BUFFER];
        va_list ap;
        va_start(ap, fmt);
        vsprintf(str, fmt, ap);
        MGlobal::displayInfo(::MString(&str[0]));
        va_end(ap);
        #endif
    }
}}

#define WSP_MAYA_ERROR(fmt, ...) wsp::maya::Error(__FILE__, __LINE__, fmt, __VA_ARGS__);
#define WSP_MAYA_WARNING(fmt, ...) wsp::maya::Warning(__FILE__, __LINE__, fmt, __VA_ARGS__);

#define WSP_MAYA_ASSERT(mstatus, ret_val, fmt, ...)             \
    if(mstatus != MS::kSuccess){                                \
        wsp::maya::Error(__FILE__, __LINE__, MString(mstatus.errorString()+MString(": ")+MString(fmt)).asChar(), __VA_ARGS__); \
        return ret_val;                                         \
    }


#define WSP_MAYA_ASSERT_RETURN(mstatus, fmt, ...) WSP_MAYA_ASSERT(mstatus, mstatus, fmt, __VA_ARGS__)

#define WSP_MAYA_ASSERT_EXIT(mstate, fmt, ...)                  \
    if(mstate != MS::kSuccess){                                 \
        wsp::maya::Error(__FILE__, __LINE__, MString(mstatus.errorString()+MString(": ")+MString(fmt)).asChar(), __VA_ARGS__); \
        exit(0);                                                \
    }
        
#define WSP_MAYA_ASSERT_FIXED_MSG_RETURN(mstatus) WSP_MAYA_ASSERT_RETURN((mstatus), "The function returned failure\n")

#define MAKE_INPUT(attr)                                        \
    WSP_MAYA_ASSERT_FIXED_MSG_RETURN(attr.setKeyable(true) );    \
    WSP_MAYA_ASSERT_FIXED_MSG_RETURN(attr.setStorable(true));   \
    WSP_MAYA_ASSERT_FIXED_MSG_RETURN(attr.setReadable(true));   \
    WSP_MAYA_ASSERT_FIXED_MSG_RETURN(attr.setWritable(true));

#define MAKE_OUTPUT(attr)                                       \
    WSP_MAYA_ASSERT_FIXED_MSG_RETURN(attr.setKeyable(false));   \
    WSP_MAYA_ASSERT_FIXED_MSG_RETURN(attr.setStorable(false));  \
    WSP_MAYA_ASSERT_FIXED_MSG_RETURN(attr.setReadable(true));   \
    WSP_MAYA_ASSERT_FIXED_MSG_RETURN(attr.setWritable(false));

#endif

#endif
