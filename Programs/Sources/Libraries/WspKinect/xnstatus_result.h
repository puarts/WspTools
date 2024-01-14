/**
 * @file xnstatus_result.h
 * @author Jun-ichi Nishikata
 */

#ifndef WSP_KINECT_XNSTATUS_RESULT_H_
#define WSP_KINECT_XNSTATUS_RESULT_H_

#include <wsp/kinect/define_kinect.h>

#ifdef USE_XN


#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif

/*
 *   Result range for image module: 0x00001000 - 0x00001fff
 */
#ifdef __cplusplus
namespace wsp{ namespace kinect{
    WSP_DEFINE_CPP_RESULT_RANGE( ResultXnStatusError, 0x00002100, 0x000021ff, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( ResultXnStatusUnknown, 0x00002101, 0x00002101, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_ERROR                    , 0x00002102, 0x00002102, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NOT_INIT                 , 0x00002103, 0x00002103, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_ALREADY_INIT             , 0x00002104, 0x00002104, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NULL_INPUT_PTR           , 0x00002105, 0x00002105, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NULL_OUTPUT_PTR          , 0x00002106, 0x00002106, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_INPUT_BUFFER_OVERFLOW    , 0x00002107, 0x00002107, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_OUTPUT_BUFFER_OVERFLOW   , 0x00002108, 0x00002108, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_INTERNAL_BUFFER_TOO_SMALL, 0x00002109, 0x00002109, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_INVALID_BUFFER_SIZE      , 0x0000210a, 0x0000210a, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NO_MATCH                 , 0x0000210b, 0x0000210b, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_IS_EMPTY                 , 0x0000210c, 0x0000210c, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_IS_NOT_EMPTY             , 0x0000210d, 0x0000210d, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_ILLEGAL_POSITION         , 0x0000210e, 0x0000210e, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NOT_IMPLEMENTED          , 0x0000210f, 0x0000210f, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NO_MODULES_FOUND         , 0x00002110, 0x00002110, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_INVALID_GENERATOR        , 0x00002111, 0x00002111, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_UNKNOWN_GENERATOR_TYPE   , 0x00002112, 0x00002112, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_INVALID_OPERATION        , 0x00002113, 0x00002113, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_MISSING_NEEDED_TREE      , 0x00002114, 0x00002114, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_CORRUPT_FILE             , 0x00002115, 0x00002115, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_BAD_PARAM                , 0x00002116, 0x00002116, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NODE_IS_LOCKED           , 0x00002117, 0x00002117, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_WAIT_DATA_TIMEOUT        , 0x00002118, 0x00002118, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_BAD_TYPE                 , 0x00002119, 0x00002119, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_UNSUPPORTED_VERSION      , 0x0000211a, 0x0000211a, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_PROPERTY_NOT_SET         , 0x0000211b, 0x0000211b, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_BAD_FILE_EXT             , 0x0000211c, 0x0000211c, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NODE_NOT_LOADED          , 0x0000211d, 0x0000211d, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NO_NODE_PRESENT          , 0x0000211e, 0x0000211e, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_BAD_NODE_NAME            , 0x0000211f, 0x0000211f, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_UNSUPPORTED_CODEC        , 0x00002120, 0x00002120, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_EOF                      , 0x00002121, 0x00002121, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_MULTIPLE_NODES_ERROR     , 0x00002122, 0x00002122, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_DEVICE_NOT_CONNECTED     , 0x00002123, 0x00002123, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NO_LICENSE               , 0x00002124, 0x00002124, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NO_SUCH_PROPERTY         , 0x00002125, 0x00002125, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NODE_ALREADY_RECORDED    , 0x00002126, 0x00002126, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_PROTO_BAD_INTERFACE      , 0x00002127, 0x00002127, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_PROTO_BAD_MSG_TYPE       , 0x00002128, 0x00002128, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_PROTO_BAD_CID            , 0x00002129, 0x00002129, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_PROTO_BAD_NODE_ID        , 0x0000212a, 0x0000212a, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_PROTO_BAD_MSG_SIZE       , 0x0000212b, 0x0000212b, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_NO_SUCH_USER             , 0x0000212c, 0x0000212c, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_USER_IS_NOT_BEING_TRACKED, 0x0000212d, 0x0000212d, WSP_RESULT_LEVEL_FATAL );
        WSP_DEFINE_CPP_RESULT_RANGE( XN_STATUS_JOINT_IS_NOT_ACTIVE      , 0x0000212e, 0x0000212e, WSP_RESULT_LEVEL_FATAL );
}}
#endif

namespace wsp{ namespace kinect{
    wsp::Result MakeResultFromXnStatus( int value )
    {
        switch( value )
        {
        case ::XN_STATUS_ERROR                    : return wsp::kinect::XN_STATUS_ERROR                    ();
        case ::XN_STATUS_NOT_INIT                 : return wsp::kinect::XN_STATUS_NOT_INIT                 ();
        case ::XN_STATUS_ALREADY_INIT             : return wsp::kinect::XN_STATUS_ALREADY_INIT             ();
        case ::XN_STATUS_NULL_INPUT_PTR           : return wsp::kinect::XN_STATUS_NULL_INPUT_PTR           ();
        case ::XN_STATUS_NULL_OUTPUT_PTR          : return wsp::kinect::XN_STATUS_NULL_OUTPUT_PTR          ();
        case ::XN_STATUS_INPUT_BUFFER_OVERFLOW    : return wsp::kinect::XN_STATUS_INPUT_BUFFER_OVERFLOW    ();
        case ::XN_STATUS_OUTPUT_BUFFER_OVERFLOW   : return wsp::kinect::XN_STATUS_OUTPUT_BUFFER_OVERFLOW   ();
        case ::XN_STATUS_INTERNAL_BUFFER_TOO_SMALL: return wsp::kinect::XN_STATUS_INTERNAL_BUFFER_TOO_SMALL();
        case ::XN_STATUS_INVALID_BUFFER_SIZE      : return wsp::kinect::XN_STATUS_INVALID_BUFFER_SIZE      ();
        case ::XN_STATUS_NO_MATCH                 : return wsp::kinect::XN_STATUS_NO_MATCH                 ();
        case ::XN_STATUS_IS_EMPTY                 : return wsp::kinect::XN_STATUS_IS_EMPTY                 ();
        case ::XN_STATUS_IS_NOT_EMPTY             : return wsp::kinect::XN_STATUS_IS_NOT_EMPTY             ();
        case ::XN_STATUS_ILLEGAL_POSITION         : return wsp::kinect::XN_STATUS_ILLEGAL_POSITION         ();
        case ::XN_STATUS_NOT_IMPLEMENTED          : return wsp::kinect::XN_STATUS_NOT_IMPLEMENTED          ();
        case ::XN_STATUS_NO_MODULES_FOUND         : return wsp::kinect::XN_STATUS_NO_MODULES_FOUND         ();
        case ::XN_STATUS_INVALID_GENERATOR        : return wsp::kinect::XN_STATUS_INVALID_GENERATOR        ();
        case ::XN_STATUS_UNKNOWN_GENERATOR_TYPE   : return wsp::kinect::XN_STATUS_UNKNOWN_GENERATOR_TYPE   ();
        case ::XN_STATUS_INVALID_OPERATION        : return wsp::kinect::XN_STATUS_INVALID_OPERATION        ();
        case ::XN_STATUS_MISSING_NEEDED_TREE      : return wsp::kinect::XN_STATUS_MISSING_NEEDED_TREE      ();
        case ::XN_STATUS_CORRUPT_FILE             : return wsp::kinect::XN_STATUS_CORRUPT_FILE             ();
        case ::XN_STATUS_BAD_PARAM                : return wsp::kinect::XN_STATUS_BAD_PARAM                ();
        case ::XN_STATUS_NODE_IS_LOCKED           : return wsp::kinect::XN_STATUS_NODE_IS_LOCKED           ();
        case ::XN_STATUS_WAIT_DATA_TIMEOUT        : return wsp::kinect::XN_STATUS_WAIT_DATA_TIMEOUT        ();
        case ::XN_STATUS_BAD_TYPE                 : return wsp::kinect::XN_STATUS_BAD_TYPE                 ();
        case ::XN_STATUS_UNSUPPORTED_VERSION      : return wsp::kinect::XN_STATUS_UNSUPPORTED_VERSION      ();
        case ::XN_STATUS_PROPERTY_NOT_SET         : return wsp::kinect::XN_STATUS_PROPERTY_NOT_SET         ();
        case ::XN_STATUS_BAD_FILE_EXT             : return wsp::kinect::XN_STATUS_BAD_FILE_EXT             ();
        case ::XN_STATUS_NODE_NOT_LOADED          : return wsp::kinect::XN_STATUS_NODE_NOT_LOADED          ();
        case ::XN_STATUS_NO_NODE_PRESENT          : return wsp::kinect::XN_STATUS_NO_NODE_PRESENT          ();
        case ::XN_STATUS_BAD_NODE_NAME            : return wsp::kinect::XN_STATUS_BAD_NODE_NAME            ();
        case ::XN_STATUS_UNSUPPORTED_CODEC        : return wsp::kinect::XN_STATUS_UNSUPPORTED_CODEC        ();
        case ::XN_STATUS_EOF                      : return wsp::kinect::XN_STATUS_EOF                      ();
        case ::XN_STATUS_MULTIPLE_NODES_ERROR     : return wsp::kinect::XN_STATUS_MULTIPLE_NODES_ERROR     ();
        case ::XN_STATUS_DEVICE_NOT_CONNECTED     : return wsp::kinect::XN_STATUS_DEVICE_NOT_CONNECTED     ();
        case ::XN_STATUS_NO_LICENSE               : return wsp::kinect::XN_STATUS_NO_LICENSE               ();
        case ::XN_STATUS_NO_SUCH_PROPERTY         : return wsp::kinect::XN_STATUS_NO_SUCH_PROPERTY         ();
        case ::XN_STATUS_NODE_ALREADY_RECORDED    : return wsp::kinect::XN_STATUS_NODE_ALREADY_RECORDED    ();
        case ::XN_STATUS_PROTO_BAD_INTERFACE      : return wsp::kinect::XN_STATUS_PROTO_BAD_INTERFACE      ();
        case ::XN_STATUS_PROTO_BAD_MSG_TYPE       : return wsp::kinect::XN_STATUS_PROTO_BAD_MSG_TYPE       ();
        case ::XN_STATUS_PROTO_BAD_CID            : return wsp::kinect::XN_STATUS_PROTO_BAD_CID            ();
        case ::XN_STATUS_PROTO_BAD_NODE_ID        : return wsp::kinect::XN_STATUS_PROTO_BAD_NODE_ID        ();
        case ::XN_STATUS_PROTO_BAD_MSG_SIZE       : return wsp::kinect::XN_STATUS_PROTO_BAD_MSG_SIZE       ();
        case ::XN_STATUS_NO_SUCH_USER             : return wsp::kinect::XN_STATUS_NO_SUCH_USER             ();
        case ::XN_STATUS_USER_IS_NOT_BEING_TRACKED: return wsp::kinect::XN_STATUS_USER_IS_NOT_BEING_TRACKED();
        case ::XN_STATUS_JOINT_IS_NOT_ACTIVE      : return wsp::kinect::XN_STATUS_JOINT_IS_NOT_ACTIVE      ();
        default:                                  return wsp::kinect::ResultXnStatusUnknown();
        }
    }
}}

#endif
#endif