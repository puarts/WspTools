/**
 * @file cl-image_filter_plugin_interface.inl
 * 
 */

#ifndef WSP_IMAGE_CLASS_IMAGE_FILTER_PLUGIN_INTERFACE_INL_
#define WSP_IMAGE_CLASS_IMAGE_FILTER_PLUGIN_INTERFACE_INL_

#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include <wsp/common/common_result.h>
#endif
#include "tpl_cl-image_filter_plugin_interface.hpp"

namespace{
    static const u32 WSP_MAJOR_VERSION_MASK = 0xffff0000;
    static const u32 WSP_MINOR_VERSION_MASK = 0x0000ff00;
    static const u32 WSP_MICRO_VERSION_MASK = 0x000000ff;
}


#ifdef USE_QT
    #define INIT_WSP_FILTER_PLUGIN_INTERFACE \
        id_(-1),\
        version_(0),\
        image_label_(NULL), \
        image_(NULL), \
        img_seq_(NULL), \
        comp_layer_list_(NULL),\
        composition_param_(NULL),\
        enable_preview_(false)
#else
    #define INIT_WSP_FILTER_PLUGIN_INTERFACE \
        id_(-1),\
        image_(NULL), \
        img_seq_(NULL), \
        comp_layer_list_(NULL),\
        composition_param_(NULL),\
        enable_preview_(false)
#endif


template<typename T>
inline wsp::ImageFilterPluginInterface<T>::ImageFilterPluginInterface() 
    :INIT_WSP_FILTER_PLUGIN_INTERFACE
{
    memset(category_,       0, MAX_LENGTH_OF_NAME);
    memset(filter_name_,    0, MAX_LENGTH_OF_NAME);
    memset(version_as_str_, 0, MAX_LENGTH_OF_NAME);
    memset(src_img_path_,   0, MAX_LENGTH_OF_PATH);
}

/*===================================================*/
/*===================================================*/
/*===================================================*/

template<typename T>
inline u16 wsp::ImageFilterPluginInterface<T>::GetMajorVersion     (void) const
{ 
    return version_ % WSP_MAJOR_VERSION_MASK; 
}

template<typename T>
inline u8  wsp::ImageFilterPluginInterface<T>::GetMinorVersion     (void) const
{
    return version_ % WSP_MINOR_VERSION_MASK; 
}

template<typename T>
inline u8  wsp::ImageFilterPluginInterface<T>::GetMicroVersion     (void) const
{ 
    return version_ % WSP_MICRO_VERSION_MASK; 
}


template<typename T>
wsp::Result wsp::ImageFilterPluginInterface<T>::GetDefaultParams( wsp::Vector<wsp::StdParam>* o_params )
{
    if( o_params==NULL ){ return wsp::ResultInvalidParameter(); }

    int num_params = param_def_list_.length();
    o_params->SetLength( num_params );
    for( int i=0; i<num_params; ++i )
    {
        switch(param_def_list_[i].param_type())
        {
        case WSP_STD_PARAM_INTEGER:
            (*o_params)[i].int_value = param_def_list_[i].default_int();
            WSP_COMMON_LOG("int_value=%d\n", (*o_params)[i].int_value);
            break;
        case WSP_STD_PARAM_FLOAT:
            (*o_params)[i].double_value = param_def_list_[i].default_double();
            WSP_COMMON_LOG("double_value=%f\n", (*o_params)[i].double_value);
            break;
        case WSP_STD_PARAM_BOOLEAN:
            (*o_params)[i].bool_value = param_def_list_[i].default_bool();
            WSP_COMMON_LOG("bool_value=%s\n", (*o_params)[i].bool_value? "true":"false");
            break;
        case WSP_STD_PARAM_POINTER_LIST:
            (*o_params)[i].int_value = param_def_list_[i].default_list_index();
            WSP_COMMON_LOG("list_index=%d\n", (*o_params)[i].int_value);
            break;
        case WSP_STD_PARAM_OUTPUT_FILEPATH:
        case WSP_STD_PARAM_INPUT_FILEPATH:
            strcpy( (*o_params)[i].file_path, param_def_list_[i].file_path() );
            WSP_COMMON_LOG("file_path=%s\n", (*o_params)[i].file_path);
        default:
            WSP_COMMON_WARNING_LOG("Unknown parameter\n");
            continue;
        }
    }
    return wsp::ResultSuccess();
}


template<typename T>
inline wsp::StdParam wsp::ImageFilterPluginInterface<T>::GetParamByID(
    const wsp::Vector<wsp::StdParam> &param_list, 
    s32 id
)
{
    int len = param_list.length();
    for(int i=0; i<len; ++i){
        WSP_COMMON_LOG( "  %d == %d\n", param_list[i].id, id );
        if(param_list[i].id==id){ return param_list[i]; }
    }
    WSP_COMMON_ERROR_LOG(
        "The parameter of ID %d is not found\n", id
        );
    return param_list[0];
}

template<typename T>
inline wsp::StdParam wsp::ImageFilterPluginInterface<T>::GetParamByName(
    const wsp::Vector<wsp::StdParam> &param_list, 
    const char *name
)
{
    int len = param_list.length();
    for(int i=0; i<len; ++i){
        if(strcmp(param_list[i].name, name)==0){ return param_list[i]; }
    }
    WSP_COMMON_ERROR_LOG("The parameter \"%s\" is not found\n", name);
    return param_list[0];
}

template<typename T>
inline wsp::StdParamDefinition* wsp::ImageFilterPluginInterface<T>::GetParamDefinitionByID( s32 id )
{
    int len = param_def_list_.length();
    for(int i=0; i<len; ++i){
        if(param_def_list_[i].param_id()==id){ return &param_def_list_[i]; }
    }
    WSP_COMMON_ERROR_LOG("The parameter definition of given ID is not found\n");
    return NULL;
}

/*===================================================*/
/*===================================================*/
/*===================================================*/


template<typename T>
inline void wsp::ImageFilterPluginInterface<T>::SetVersion( u32 version )
{
    version_ = version;
    sprintf( version_as_str_, "%u.%u.%u", GetMajorVersion(), GetMinorVersion(), GetMicroVersion() );
}

template<typename T>
void wsp::ImageFilterPluginInterface<T>::SetVersion( u16 major_version, u8 minor_version, u8 micro_version )
{
    version_ = (major_version<<16) | (minor_version<<8) | micro_version;
    sprintf( version_as_str_, "%u.%u.%u", GetMajorVersion(), GetMinorVersion(), GetMicroVersion() );
}

template<typename T>
inline void wsp::ImageFilterPluginInterface<T>::SetDefaultParams(wsp::Vector<wsp::StdParam> &in_params)
{
    for(int i=0; i<param_def_list_.length(); ++i)
    {
        WSP_COMMON_LOG("%s: ", param_def_list_[i].name());
       switch(param_def_list_[i].param_type())
        {
        case WSP_STD_PARAM_INTEGER:
            param_def_list_[i].set_default_int(in_params[i].int_value);
            WSP_COMMON_LOG("int_value=%d\n", param_def_list_[i].default_int());
            break;
        case WSP_STD_PARAM_FLOAT:
            param_def_list_[i].set_default_double(in_params[i].double_value);
            WSP_COMMON_LOG("double_value=%d\n", param_def_list_[i].default_double());
            break;
        case WSP_STD_PARAM_BOOLEAN:
            param_def_list_[i].set_default_bool(in_params[i].bool_value);
            WSP_COMMON_LOG("bool_value=%s\n", param_def_list_[i].default_bool()? "true":"false");
            break;
        case WSP_STD_PARAM_POINTER_LIST:
            param_def_list_[i].set_default_list_index(in_params[i].int_value);
            WSP_COMMON_LOG("list_index=%d\n", param_def_list_[i].default_list_index());
            break;
        case WSP_STD_PARAM_INPUT_FILEPATH:
            param_def_list_[i].set_file_path(in_params[i].file_path);
            WSP_COMMON_LOG("file_path=%s\n", param_def_list_[i].file_path());
            break;
        default:
            WSP_COMMON_WARNING_LOG("Unknown parameter\n");
            continue;
        }
    }
}

template<typename T>
inline wsp::Result wsp::ImageFilterPluginInterface<T>::SetSrcImagePath ( const char *src_path    )
{ 
    if(strlen(src_path)>=MAX_LENGTH_OF_PATH)
    { 
        WSP_COMMON_ERROR_LOG("Too long path\n"); 
        return wsp::ResultTooLargeSize(); 
    }
    strcpy(src_img_path_, src_path);
    return wsp::ResultSuccess();
}


#endif 

