/**
 * @file multimedia_result.h
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_MULTIMEDIA_MULTIMEDIA_RESULT_H_
#define WSP_MULTIMEDIA_MULTIMEDIA_RESULT_H_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif

/*
 *   Result range for multimedia module: 0x00006000 - 0x00006fff
 */
#ifdef __cplusplus
namespace wsp{ namespace mm{
    WSP_DEFINE_CPP_RESULT_RANGE( ResultInvalidUsage, 0x00006000, 0x000060ff, WSP_RESULT_LEVEL_USAGE );
        WSP_DEFINE_CPP_RESULT( ResultIsExternalData,     0x00006001, WSP_RESULT_LEVEL_USAGE );
        WSP_DEFINE_CPP_RESULT( ResultUnsupportedType,    0x00006002, WSP_RESULT_LEVEL_USAGE );
        WSP_DEFINE_CPP_RESULT( ResultUnmatchedMediaType, 0x00006003, WSP_RESULT_LEVEL_USAGE );

    WSP_DEFINE_CPP_RESULT_RANGE( ResultFatalError, 0x00001100, 0x000011ff, WSP_RESULT_LEVEL_FATAL );
}}
#endif


#endif